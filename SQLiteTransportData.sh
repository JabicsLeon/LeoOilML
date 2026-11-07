#!/opt/homebrew/bin/bash

#===============================================
#================== Function ===================
#===============================================

#🗂️  SQLite3 create db 🗂️ 
create_sqlite_db()
{
sqlite3 database.db << EDF
PRAGMA foreign_keys = ON;
PRAGMA journal_mode = WAL;

CREATE TABLE Header
(
	place TEXT PRIMARY KEY,
	cord_sys TEXT
);

CREATE TABLE Well
(
	id INTEGER PRIMARY KEY,
	place_well TEXT NOT NULL,
	name TEXT NOT NULL UNIQUE,
	depth_mnemonic TEXT,
	x INTEGER,
	y INTEGER,
	FOREIGN KEY (place_well) REFERENCES Header(place)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE Methods
(
	id INTEGER NOT NULL,
	method TEXT NOT NULL,
	unit TEXT,
	type TEXT,
	PRIMARY KEY (id, method),
	FOREIGN KEY (id) REFERENCES Well(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE Data_well
(
	id INTEGER NOT NULL,
	method TEXT NOT NULL,
	z REAL NOT NULL,
	value REAL NOT NULL,
	PRIMARY KEY (id, method, z),
	FOREIGN KEY (id, method) REFERENCES Methods(id, method)
		ON DELETE CASCADE
		ON UPDATE CASCADE,
	FOREIGN KEY (id) REFERENCES Well(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);


CREATE TABLE Interpretation
(
	place_inter TEXT NOT NULL,
	rock TEXT NOT NULL,
	value INTEGER NOT NULL,
	PRIMARY KEY (place_inter, rock),
	FOREIGN KEY (place_inter) REFERENCES Header(place)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE Data_2D
(
	place_2d TEXT NOT NULL,
	method TEXT NOT NULL,
	x REAL NOT NULL,
	y REAL NOT NULL,
	value REAL NOT NULL,
	unit TEXT,
	FOREIGN KEY (place_2d) REFERENCES Header(place)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE Data_3D
(
	place_3d TEXT NOT NULL,
	method TEXT NOT NULL,
	attribute TEXT NOT NULL,
	x REAL NOT NULL,
	y REAL NOT NULL,
	z REAL NOT NULL,
	value REAL NOT NULL,
	unit TEXT,
	FOREIGN KEY (place_3d) REFERENCES Header(place)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE INDEX id_well_place 		ON Well(place_well);
#CREATE INDEX id_inter_place		ON Interpretation(place_inter);
CREATE INDEX id_2d_place		ON Data_2D(place_2d);
CREATE INDEX id_3d_place		ON Data_3D(place_3d);

CREATE VIEW header_stats AS
SELECT h.place,
	(SELECT COUNT(*) FROM Well w WHERE w.place_well = h.place) AS n_wells,
	(SELECT COUNT(*) FROM Data_2d d WHERE d.place_2d = h.place) AS n_2d,
	(SELECT COUNT(*) FROM Data_3d d WHERE d.place_3d = h.place) AS n_3d
FROM Header h;

CREATE VIEW method_stats AS
SELECT method, COUNT(*) AS n FROM Methods
GROUP BY method
ORDER BY n DESC;

EDF
}


#🗂️   SQLite3 clean db 🗂️
clean_sqlite_db()
{
local DBPlace="$1"

sqlite3 "$DBPlace" << EDF

PRAGMA foreign_keys = ON;

BEGIN;

DELETE FROM Well WHERE NOT EXISTS 
(SELECT 1 FROM Data_well WHERE Data_well.id = Well.id);

UPDATE Header SET well =
(SELECT COUNT(*) FROM Well WHERE Well.place_well = Header.place);

COMMIT;

EDF
}



#🐍 Read DLIS 🐍
read_dlis_and_send_to_db()
{
local dbplace="$1"
local file="$2"
local Name_well="$3"
local id="$4"

python3 << EDF
import sys

from collections import defaultdict

import pandas as pd
import numpy as np
import math

import sqlite3

from dlisio import dlis
from dlisio.common import ErrorHandler, Actions

eh = ErrorHandler(critical=Actions.LOG_ERROR)

def dlis_frame_to_dataframe(frame):
	curves = frame.curves()
	data = defaultdict(list)
	for name in curves.dtype.names:
		field = curves[name]
		field_dtype = curves.dtype[name]

		if field_dtype.shape == ():
			data[name] = field
		else:
			if field.ndim == 2:
				flat = np.array([np.array(x).flatten() for x in field])
				for i in range(flat.shape[1]):
					data[f"{name}_{i+1}"] = flat[:, i]

	df = pd.DataFrame(data)
	return df


def frame_to_sqlile(cur, frame):
	mn  = [ch.name  for ch in frame.channels if ch.name is not None]
	uni = [ch.units for ch in frame.channels if ch.name is not None]
	meas = dict(zip(mn, uni))
	#df = dlis_frame_to_dataframe(frame)
	depth_name = ""
	for m in mn:
		if -1 != m.find("DEP"):
			depth_name = m
			break
			#if depth_name == "":
			#	depth_name = m
			#elif len(df[depth_name]) < len(df[m]):
			#	depth_name = m
	if depth_name == "":
		print(f"For $Name_well well 'DEP' not found!")
		sys.exit(1)

	cur.execute(
		f"UPDATE Well SET depth_mnemonic = '{depth_name}' WHERE  id = $id"
	)


	df = dlis_frame_to_dataframe(frame)
	
	info_rows = [ ("$id", m, meas.get(m)) for m in df.columns ]
	cur.executemany(
		"INSERT OR IGNORE INTO Methods (id, method, unit) VALUES (?, ?, ?)",
		info_rows,
	)
	

	data_rows = [("$id", m, depth, value)  for m in df.columns  if m != depth_name for depth, value in zip(df[depth_name], df[m]) if pd.notna(value) and math.isfinite(value)]
	cur.executemany(
		"INSERT INTO Data_well (id, method, z, value) VALUES (?, ?, ?, ?)"
		"ON CONFLICT (id, method, z) DO NOTHING", 
		data_rows, 
	)
	
def main():
	con = sqlite3.connect("$dbplace")
	con.execute("PRAGMA foreign_keys = ON")
	cur = con.cursor()
	try:
		with dlis.load("$file", error_handler=eh) as files:
			for f in files:
				for frame in f.frames:
					frame_to_sqlile(cur, frame)
					con.commit()					
	except Exception as e:
		print(f"Skip $file: {type(e).__name__}: {e}")
		sys.exit(1)

if __name__=="__main__":
	main()
EDF
}

#🐍 Read LAS 🐍
read_las_and_send_to_db()
{
local dbplace="$1"
local file="$2"
local Name_well="$3"
local id="$4"

python3 << EDF
import sys

from collections import defaultdict

import pandas as pd
import numpy as np
import math

import sqlite3

import lasio

def file_to_sqlite(cur, file):
	try:
		las = lasio.read(file, ignore_header_errors=True)
		mn  = [curve.mnemonic for curve in las.curves]
		uni = [curve.unit for curve in las.curves]
		meas = dict(zip(mn, uni))
		depth_name = ""
		for m in mn:
			if -1 != m.find("DEP"):
				depth_name = m
				break
				#if depth_name == "":
				#	 depth_name = m
				#elif len(las.curves[depth_name]) < len(las.curves[m]):
				#	depth_name = m
		if depth_name == "":
			print(f"For $Name_well well 'DEP' not found!")
			sys.exit(1)

		cur.execute(
			f"UPDATE Well SET depth_mnemonic = '{depth_name}' WHERE  id = $id"
		)

		info_rows = [ ("$id", m, meas[m]) for m in mn ]
		cur.executemany(
			"INSERT OR IGNORE INTO Methods (id, method, unit) VALUES (?, ?, ?)",
			info_rows,
		)
		

		data_rows = [("$id", m, depth, value) for m in mn if m != depth_name for depth, value in zip(las.curves[depth_name], las.curves[m]) if pd.notna(value) and math.isfinite(value)]
		cur.executemany(
			"INSERT INTO Data_well (id, method, z, value) VALUES (?, ?, ?, ?)"
			"ON CONFLICT (id, method, z) DO NOTHING",
			data_rows,
		)

	except Exception as e:
		print(f"Skip {file}: {type(e).__name__}: {e}")
		sys.exit(1)

def main():
	con = sqlite3.connect("$dbplace")
	con.execute("PRAGMA foreign_keys = ON")
	cur = con.cursor()
	file_to_sqlite(cur, "$file")
	con.commit()

if __name__=="__main__":
	main()
EDF
}

🛡️ Bash Guard 🛡️
space_disk_check()
{
local path="$1"
local min_mb="${2:-10000}"
local avail_kb avail_mb
avail_kb=$(df -k  "$path" 2>/dev/null | awk 'NR==2 {print $4}')
avail_mb=$(( ${avail_kb:-0} / 1024 ))
if [ $avail_mb -lt "$min_mb" ]; then
	echo "❌ ABORT: only ${avail_mb} MB free on $path (need ${min_mb} MB)" >& 2
	return 1
fi
return 0
}






#===============================================
#===================== Main ====================
#===============================================
#🐚==Bash part in code==🐚
shopt -s nullglob
shopt -s globstar

if [ 2 -lt $(ps aux | grep -E "[S]QLiteTransportData\.sh" | wc -l) ]; then
	exit 1
fi

CorDIR=`pwd`
DATA_PLACE="$1"

if [[ -n "$1" ]]; then
	cd "$DATA_PLACE" || { echo "Dirictory not found" >&2; exit 2; }
else
	DATA_PLACE="/Volumes/ADATA"
	cd "$DATA_PLACE" || { echo "Dirictory not found" >&2; exit 2; }
fi

ExDisk=`pwd`
DATA_PLACE=$ExDisk

ls database.db &> /dev/null || { create_sqlite_db; echo "Create database"; }

SQLitePlace="$ExDisk/database.db"

if cd "$DATA_PLACE/PEPS_SA/downloaded_files"; then
	Au="$DATA_PLACE/PEPS_SA/downloaded_files"
	echo "🚩 Go to Australia dir: $Au"

	if [ -z "$(sqlite3 "$SQLitePlace" "SELECT 1 FROM Header WHERE place = 'Australia';")" ]; then
		sqlite3 "$SQLitePlace" "INSERT INTO Header (place) VALUES ('Australia');"
	fi

	for sub_dir in */; do
		space_disk_check "$DATA_PLACE" 10000 || exit 1

		if [ -z "$(sqlite3 "$SQLitePlace" "SELECT 1 FROM Well WHERE name  = '${sub_dir%/}' LIMIT 1;")" ] && cd "$Au/$sub_dir"; then
			echo "|  ⛏️ processing for ${sub_dir%/} well"
			sqlite3 "$SQLitePlace" "INSERT INTO Well (place_well, name) VALUES ('Australia','${sub_dir%/}');"
			well_id=$(sqlite3 "$SQLitePlace" "SELECT id FROM Well WHERE name='${sub_dir%/}' LIMIT 1;")			

			check=0
			
			# 🚩Dlis processing
			for file in **/*.dlis **/*.DLIS;
			do
				space_disk_check "$DATA_PLACE" 5000 || exit 1
				echo "|  |  ⏩️ Start processing with file: $file"
				read_dlis_and_send_to_db "$SQLitePlace" "$file" "${sub_dir%/}" "$well_id"  && check=1
				echo "|  |  ✅ End processing with file: $file"
			done
	
			# 🚩LAS processing
			for file in **/*.las **/*.LAS;
			do
				space_disk_check "$DATA_PLACE" 5000 || exit 1
				echo "|  |  ⏩️ Start processing with $file file"
				read_las_and_send_to_db "$SQLitePlace" "$file" "${sub_dir%/}" "$well_id" && check=1
				echo "|  |  ✅ End processing with file: $file"
			done
	
			if [[ 0 -eq "$check" ]]; then
				sqlite3 "$SQLitePlace" "PRAGMA foreign_keys = ON;  DELETE FROM Well WHERE id = $well_id;"
			fi

			cd "$Au"
		fi
	done
	echo "✅ Australia has done, dir: $Au"
fi


cd "$DATA_PLACE" || exit 2

for sub_dir in */; do
	[[ "$DATA_PLACE/PEPS_SA/" == "$DATA_PLACE/$sub_dir" ]] && continue

	if [ -z "$(sqlite3 "$SQLitePlace" "SELECT 1 FROM Header WHERE place = '${sub_dir%/}';")" ]; then
		sqlite3 "$SQLitePlace" "INSERT INTO Header (place) VALUES ('${sub_dir%/}');"
	fi

	if cd "$DATA_PLACE/$sub_dir"; then
		echo "🚩 Go to dir: ${sub_dir%/}"		

		 # 🚩Dlis processing
		for file in **/*.dlis **/*.DLIS;
		do
			check=0

			space_disk_check "$DATA_PLACE" 5000 || exit 1
			if [ -z "$(sqlite3 "$SQLitePlace" "SELECT 1 FROM Well WHERE name = '${file%.*}' LIMIT 1;")" ]; then
				sqlite3 "$SQLitePlace" "INSERT INTO Well (place_well, name) VALUES ('${sub_dir%/}','${file%.*}');"
				echo "|  ⛏️  processing for ${file%.*} well"
				well_id=$(sqlite3 "$SQLitePlace" "SELECT id FROM Well WHERE name='${file%.*}' LIMIT 1;")
				echo "|  |  ⏩️ Start processing with file: $file"
				read_dlis_and_send_to_db "$SQLitePlace" "$file" "${file%.*}" "$well_id" && \
				{ echo "|  |  ✅ End processing with file: $file"; check=1; }

				if [[ 0 -eq "$check" ]]; then
					sqlite3 "$SQLitePlace" "PRAGMA foreign_keys = ON;  DELETE FROM Well WHERE id = $well_id;"
				fi
			fi
		done
		# 🚩LAS processing
		for file in **/*.las **/*.LAS;
		do
			check=0

			space_disk_check "$DATA_PLACE" 5000 || exit 1
			if [ -z "$(sqlite3 "$SQLitePlace" "SELECT 1 FROM Well WHERE name = '${file%.*}' LIMIT 1;")" ]; then
				sqlite3 "$SQLitePlace" "INSERT INTO Well (place_well, name) VALUES ('${sub_dir%/}','${file%.*}');"
				echo "|  ⛏️   processing for ${file%.*} well"
				well_id=$(sqlite3 "$SQLitePlace" "SELECT id FROM Well WHERE name='${file%.*}' LIMIT 1;")
				echo "|  |  ⏩️ Start processing with file: $file"
				read_las_and_send_to_db "$SQLitePlace" "$file" "${file%.*}" "$well_id" && \
				{ echo "|  |  ✅ End processing with file: $file"; check=1; }
				
				if [[ 0 -eq "$check" ]]; then
					sqlite3 "$SQLitePlace" "PRAGMA foreign_keys = ON;  DELETE FROM Well WHERE id = $well_id;"
				fi
			fi
		done
		echo "✅ ${sub_dir%/} has done, dir: $sub_dir"
	else
		echo "Skip $DATA_PLACE/$sub_dir" >&2
	fi
done

clean_sqlite_db "$SQLitePlace"

cd "$CorDIR"
