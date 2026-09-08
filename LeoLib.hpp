#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <iterator>
#include <random>
#include <numbers>
#include <complex>
#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>
#include <vector>
#include <map>
#include <string>
#include <thread>

//===============================================================================Vector_modul=========================================================================================

namespace leo
{
	template<class T> class vector;// : std::vector<T>;

	template<class T1, class T2>
	auto operator+(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() + std::declval<T2>()) >;

	template<class T1, class T2>
	auto operator+=(vector<T1>& a, const vector<T2>& b) -> vector< T1 >;

	template<class T1, class T2>
	auto operator-(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() - std::declval<T2>()) >;

	template<class T1, class T2>
	auto operator-=(vector<T1>& a, const vector<T2>& b) -> vector< T1 >;

	template<class T1, class T2>
	auto operator*(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() * std::declval<T2>()) >;

	template<class T1, class T2>
	auto operator*=(vector<T1>& a, const vector<T2>& b) -> vector< T1 >&;

	template<class T1, class T2>
	auto operator/(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() / std::declval<T2>()) >;

	template<class T1, class T2>
	auto operator/=(vector<T1>& a, const vector<T2>& b) -> vector< T1 >&;

	template<class T1, class T2>
	bool operator==(const vector<T1>& a, const vector<T2>& b);


	template<class T1, class Scalar>
	auto operator+(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() + std::declval<Scalar>()) >;

	template<class T1, class Scalar>
	auto operator+(Scalar b, const vector<T1>& a) -> vector< decltype(std::declval<Scalar>() + std::declval<T1>()) >;

	template<class T1, class Scalar>
	auto operator+=(vector<T1>& a, Scalar b) -> vector< T1 >;

	template<class T1, class Scalar>
	auto operator-(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() - std::declval<Scalar>()) >;

	template<class T1, class Scalar>
	auto operator-(Scalar b, const vector<T1>& a) -> vector< decltype(std::declval<Scalar>() - std::declval<T1>()) >;

	template<class T1, class Scalar>
	auto operator-=(vector<T1>& a, Scalar b) -> vector< T1 >;

	template<class T1, class Scalar>
	auto operator*(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() * std::declval<Scalar>()) >;

	template<class T1, class Scalar>
	auto operator*(Scalar b, const vector<T1>& a) -> vector< decltype(std::declval<Scalar>() * std::declval<T1>()) >;

	template<class T1, class Scalar>
	auto operator*=(vector<T1>& a, Scalar b) -> vector< T1 >&;

	template<class T1, class Scalar>
	auto operator/(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() / std::declval<Scalar>()) >;

	template<class T1, class Scalar>
	auto operator/=(vector<T1>& a, Scalar b) -> vector< T1 >&;


	template<class T>
	std::ostream& operator<<(std::ostream& os, const vector<T>& v);

	template<class T>
	vector<T> operator>>(std::istream& is, vector<T>& v);


	template<class T>
	class vector : public std::vector<T>
	{
		public:
			using std::vector<T>::vector;

			T abs();

			T mean();

			T variation();

			T msd();

			T moda();

			T amount();

			T amount_abs();

			template<class U>
			vector<U> cast_to() const;

			vector projection(vector& vec);

			static vector<T> randomvector(size_t N, T MinVal = 0, T MaxVal = 1);
	};

	template<typename Iteretir1, typename Iteretir2>
	auto scalarmult(Iteretir1 beg1, Iteretir1 end1, Iteretir2 beg2, Iteretir2 end2);

	template<typename cont1, typename cont2>
	auto scalarmult(cont1 c1, cont2 c2);

	template<class T, typename... Args>
	vector<T> vecmult(Args&&... args);

	template<class T, typename... Args>
	vector<T> plane(Args&&... args);

	template<class T>
	vector<T> liner_interpolation(vector<T>& vec);


	//===================================================Methods_Realisation=================================================

	template<class T>
	T vector<T>::abs()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::abs : vector is empty!");

		T value = 0;

		for (auto& it : *this)
		{
			value += it * it;
		}

		value = std::sqrt(value);

		return value;
	}

	template<class T>
	T vector<T>::mean()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::mean : vector is empty!");		

		T value = 0;

		for (auto& it : *this)
		{
			value += it;
		}

		value /= this -> size();

		return value;
	}

	template<class T>
	T vector<T>::variation()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::variation : vector is empty!");

		T value = 0;

		T mean = this -> mean();

		for (auto& it : *this)
		{
			T el =  it - mean;
			value += el * el;
		}
		
		size_t N = this -> size();

		value /= ( (N > 1) ? N - 1 : N);

		return value;
	}

	template<class T>
	T vector<T>::msd()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::msd : vector is empty!");

		T value = this -> variation();

		value = std::sqrt(value);

		return value;
	}

	template<class T>
	T vector<T>::moda()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::moda : vector is empty!");

		std::map<T, int> frequency;
		for (const auto& it : *this)
		{
			frequency[it]++;
		}

		T most_frequent = this -> front();
		int max_count = 0;

		for (const auto& pair : frequency)
		{
			if (pair.second > max_count)
			{
				max_count = pair.second;
				most_frequent = pair.first;
			}
		}

		return most_frequent;
	}

	template<class T>
	T vector<T>::amount()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::amount : vector is empty!");

		T value = 0;

		for (auto& it : *this)
		{
			value += it;
		}

		return value;
	}

	template<class T>
	T vector<T>::amount_abs()
	{
		if (!(this -> size())) throw std::invalid_argument("leo::vector<T>::amount : vector is empty!");

		T value = 0;

		for (auto& it : *this)
		{
			value += std::abs(it);
		}

		return value;
	}


	template<class T>
	template<class U>
	vector<U> vector<T>::cast_to() const
	{
		size_t N = this -> size();

		vector<U> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = static_cast<U>((*this)[i]);
		}

		return vec;
	}

	template<class T>
	static vector<T> vector<T>::randomvector(size_t N, T MinVal, T MaxVal)
	{
		vector<T> X(N);
		std::mt19937 gen(std::random_device{}());
		std::uniform_real_distribution<double> dist(MinVal, MaxVal);
		std::generate(X.begin(), X.end(), [&gen, &dist] { return dist(gen); });

		return X;
	}

	//===================================================Operators_Realisation=================================================		
	
	template<class T1, class T2>
	auto operator+(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() + std::declval<T2>()) >
	{
		size_t N = a.size();

		if (N != b.size()) throw std::invalid_argument("leo::vector::operator+: sizes of vectors not equel");

		using result_type = decltype(std::declval<T1>() + std::declval<T2>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			 vec[i] = a[i] + b[i];
		}

		return vec;
	}

        template<class T1, class T2>
        auto operator+=(vector<T1>& a, const vector<T2>& b)-> vector< T1 >
	{
		a = (a + b).template cast_to<T1>();

		return a;
	}

        template<class T1, class T2>
        auto operator-(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() - std::declval<T2>()) >
	{
		size_t N = a.size();

		if (N != b.size()) throw std::invalid_argument("leo::vector::operator-: sizes of vectors not equel");

		using result_type = decltype(std::declval<T1>() - std::declval<T2>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = a[i] - b[i];
		}

		return vec;
	}

        template<class T1, class T2>
        auto operator-=(vector<T1>& a, const vector<T2>& b) -> vector< T1  >
	{
		a = (a - b).template cast_to<T1>();

		return a;
	}

        template<class T1, class T2>
        auto operator*(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() * std::declval<T2>()) >
	{
		size_t N = a.size();

		if (N != b.size()) throw std::invalid_argument("leo::vector::operator*: sizes of vectors not equel");

		using result_type = decltype(std::declval<T1>() * std::declval<T2>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = a[i] * b[i];
		}

		return vec;
	}

        template<class T1, class T2>
	auto operator*=(vector<T1>& a, const vector<T2>& b) -> vector< T1 >&
	{
		a = (a * b).template cast_to<T1>();

		return a;
	}

        template<class T1, class T2>
        auto operator/(const vector<T1>& a, const vector<T2>& b) -> vector< decltype(std::declval<T1>() / std::declval<T2>()) >
	{
		size_t N = a.size();

		if (N != b.size()) throw std::invalid_argument("leo::vector::operator/: sizes of vectors not equel");

		using result_type = decltype(std::declval<T1>() / std::declval<T2>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			if (b[i] == 0) throw std::domain_error("leo::vector::operator/ : devision by zero");

			vec[i] = a[i] / b[i];
		}

		return vec;
	}

        template<class T1, class T2>
        auto operator/=(vector<T1>& a, const vector<T2>& b) -> vector< T1 >&
	{
		a = (a / b).template cast_to<T1>();

		return a;
	}

	template<class T1, class T2>
	bool operator==(const vector<T1>& a, const vector<T2>& b)
	{
		size_t N = a.size();

		if (N != b.size()) return false;

		for (size_t i=0; i < N; ++i)
		{
			if (a[i] != b[i]) return false;
		}

		return true;
	}


	template<class T1, class Scalar>
	auto operator+(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() + std::declval<Scalar>()) >
	{
		size_t N = a.size();

		using result_type = decltype(std::declval<T1>() + std::declval<Scalar>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = a[i] + b;
		}

		return vec;
	}

	template<class T1, class Scalar>
	auto operator+(Scalar b, const vector<T1>& a) -> vector< decltype(std::declval<Scalar>() + std::declval<T1>()) >
	{
		return a + b;
	}

	template<class T1, class Scalar>
	auto operator+=(vector<T1>& a, Scalar b) -> vector< T1  >&
	{
		a = (a + b).template cast_to<T1>();

		return a;
	}

	template<class T1, class Scalar>
	auto operator-(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() - std::declval<Scalar>()) >
	{
		size_t N = a.size();

		using result_type = decltype(std::declval<T1>() - std::declval<Scalar>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = a[i] - b;
		}

		return vec;
	}

	template<class T1, class Scalar>
	auto operator-=(vector<T1>& a, Scalar b) -> vector< T1 >&
	{
		a = (a - b).template cast_to<T1>();

		return a;
	}

	template<class T1, class Scalar>
	auto operator*(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() * std::declval<Scalar>()) >
	{
		size_t N = a.size();

		using result_type = decltype(std::declval<T1>() *  std::declval<Scalar>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = a[i] * b;
		}

		return vec;
	}

	template<class T1, class Scalar>
	auto operator*(Scalar b, const vector<T1>& a) -> vector< decltype(std::declval<Scalar>() * std::declval<T1>()) >
	{
		return a * b;
	}

	template<class T1, class Scalar>
	auto operator*=(vector<T1>& a, Scalar b) -> vector< T1 >&
	{
		a = (a * b).template cast_to<T1>();

		return a;
	}

	template<class T1, class Scalar>
	auto operator/(const vector<T1>& a, Scalar b) -> vector< decltype(std::declval<T1>() / std::declval<Scalar>()) >
	{
		if (b == 0) throw std::domain_error("leo::vector::operator/ : devision by zero");

		size_t N = a.size();

		using result_type = decltype(std::declval<T1>() / std::declval<Scalar>());

		vector<result_type> vec(N);

		for (size_t i=0; i < N; ++i)
		{
			vec[i] = a[i] / b;
		}

		return vec;
	}

	template<class T1, class Scalar>
	auto operator/=(vector<T1>& a, Scalar b) -> vector< T1 >&
	{
		a = (a / b).template cast_to<T1>();

		return a;
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, const vector<T>& v)
	{
		for (auto& it : v)
		{
			os << it << "\t";
		}

		return os;
	}

	//===================================================Same_Function_Realisation=================================================

	template<typename Iteretir1, typename Iteretir2>
	auto scalarmult(Iteretir1 beg1, Iteretir1 end1, Iteretir2 beg2, Iteretir2 end2)
	{
		auto len = std::distance(beg1, end1);

		if (len != std::distance(beg2, end2)) throw std::invalid_argument("leo::scalarmult: sizes of vectors not equel");

		auto value = 0.0;

		for (size_t i=0; i < len; ++i)
		{
			value += *(beg1 + i) * *(beg2 + i);
		}
		
		return value;
	}

	template<typename cont1, typename cont2>
	auto scalarmult(cont1 c1, cont2 c2)
	{
		return scalarmult(c1.begin(), c1.end(), c2.begin(), c2.end());
	}

}

//===============================================================================Matrix_modul=========================================================================================

namespace leo{

	template<class T> class matrix;

	template<class T1,class T2> 
	auto operator+(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() + std::declval<T2>()) >;

	template<class T1, class T2>
	auto operator-(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() - std::declval<T2>()) >;

	template<class T, class Scalar> 
	auto operator+(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >;

	template<class T, class Scalar>
	auto operator+(Scalar b ,const matrix<T>& a) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >;

	template<class T, class Scalar>
	auto operator-(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() - std::declval<Scalar>()) >;

	template<class T, class Scalar>
	auto  operator-(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() - std::declval<T>()) >;

	template<class T, class Scalar> 
	auto  operator*(const matrix<T>& a, T b) -> matrix< decltype(std::declval<T>() * std::declval<Scalar>()) >;

	template<class T, class Scalar> 
	auto operator*(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() * std::declval<T>()) >;

	template<class T, class Scalar>
	auto operator/(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() / std::declval<Scalar>()) >;

	template<class T>  
	std::ostream& operator<<(std::ostream& os, const matrix<T>& m);

	template<class T> 
	matrix<T>& operator>>(std::istream& is, matrix<T>& m);

	
	enum class SM{
		row,
		col,
		mat,
	};

	struct Rows_tag {};
	struct Cols_tag {};

	inline constexpr Rows_tag Rows {};
	inline constexpr Cols_tag Cols {};




	//===========================Class_Matrix========================================
	template<class T>
	class matrix{
	private:
			std::vector<std::vector<T>> MATRIX;
			size_t col, row, mat;
			//std::vector<std::vector<std::string>> LABELS;
	public:
		friend std::ostream& operator<< <>(std::ostream& os, const matrix<T>& m);
		friend matrix<T>& operator>> <>(std::istream& is, matrix<T>& m);

			matrix(size_t r, size_t c, T value) : row(r), col(c) 
			{
				MATRIX.resize(row, std::vector<T>(col, value));
				mat = row * col;
			}

			matrix(size_t r, size_t c) : row(r), col(c) {
				MATRIX.resize(row, std::vector<T>(col, 0));
				mat = row * col;
				//LABELS.resize(2);
				//LABELS[0].reserve(row);
				//LABELS[1].reserve(col);
			}

			matrix() : row(0), col(0) 
			{
				mat = 0;
			}
			
			matrix(const matrix<T>& A) : row(A.row), col(A.col), mat(A.mat), MATRIX(A.MATRIX)/*, LABELS(A.LABELS)*/ {}


			matrix<T> operator=(const matrix<T>& A){
				if (this != &A){
					row = A.row;
					col = A.col;
					mat = A.mat;
					MATRIX = A.MATRIX;
					//LABELS = A.LABELS;
				}
				return *this;
			}


			matrix(matrix<T>&& A) noexcept : row(A.row), col(A.col), mat(A.mat), MATRIX(std::move(A.MATRIX))/*, LABELS(std::move(A.LABELS))*/{ A.row = A.col = A.mat = 0; }

			matrix<T>& operator=(matrix<T>&& A) noexcept {
				if (this != &A){
					row = A.row;
					col = A.col;
					mat = A.mat;
					MATRIX = std::move(A.MATRIX);
					//LABELS = std::move(A.LABELS);

					A.row = A.col = A.mat = 0;
				}
				return *this;
			}

			std::vector<T>& operator[](size_t r){
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r];
			}

			const std::vector<T>& operator[](size_t r) const {
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r];
                        }

			/*std::string& row_label(size_t i){
				return &LABELS[0][i];
			}

			std::string& col_label(size_t i){
				return &LABELS[1][i];
			}*/

			size_t size(SM n) const {
				switch(n){
					case SM::col:
						return col;
					case SM::row:
						return row;
					case SM::mat:
						return mat;
				}
				return 0;
			}

			size_t size_col() const { return col; }
			
			size_t size_row() const { return row; }

			size_t size() const { return mat; }

			size_t size(Cols_tag) { return col; }

			size_t size(Rows_tag) { return row; }
			
	public:
                class iterator_horizontal;
                class iterator_vertical;
    
                iterator_horizontal h_begin();
                iterator_horizontal h_end();

                iterator_vertical v_begin();
                iterator_vertical v_end();

                auto Column(size_t k) const {
                        return ColumnRange{this, k}; 
                }   

                auto Row(size_t k) const {
                        return RowRange{this, k}; 
                }   


		auto AllColumn(){
			return AllColumnRange{this};
		}

		auto AllRow(){
			return AllRowRange{this};
		}

        private:
                struct ColumnRange {
                        const matrix<T>* M;
                        size_t col_idx;

                        auto begin() { return iterator_vertical(const_cast<matrix<T>*>(M), 0, col_idx); }
                        auto end() { return begin() + M->size_row(); }
                };  

                struct RowRange {
                        const matrix<T>* M;
                        size_t row_idx;

                        auto begin() { return iterator_horizontal(const_cast<matrix<T>*>(M), row_idx, 0); }
                        auto end() { return begin() + M->size_col(); }
                };  

		struct AllColumnRange{
			const matrix<T>* M;
			
			auto begin() { return iterator_vertical(const_cast<matrix<T>*>(M), 0); }
			auto end() { return begin() + M->size(); }
		};

		struct AllRowRange{
			const matrix<T>* M;

			auto begin() { return iterator_horizontal(const_cast<matrix<T>*>(M), 0); }
			auto end() { return begin() + M->size(); }
		};
		

        public:



			void resize(size_t nrow, size_t ncol, T value = 0){
				MATRIX.resize(nrow, std::vector<T>(ncol,value));
				row = nrow;
				col = ncol;
				mat = row * col;
				
			}

			bool is_square() const { return row == col; }

			bool is_symmetric() const {
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						if(MATRIX[i][j] != MATRIX[j][i]) return false;
					}
				}
				return true;
			}

			bool is_identity(){
				if(!is_square()) return false;
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						if (i == j) { if (MATRIX[i][j] != 1) return false; }
						else{ if (MATRIX[i][j] != 0) return false; }
					}
				}
				return true;
			}

			void swap_row(size_t a, size_t b){
				if ( a >= row ||  b >= row ) throw std::invalid_argument("Row index out of range");

				std::swap(MATRIX[a], MATRIX[b]);
			}

			void swap_col(size_t a, size_t b){
				if ( a >= col ||  b >= col ) throw std::invalid_argument("Column index out of range");
				
				for(size_t i=0; i < row; ++i){
					std::swap(MATRIX[i][a], MATRIX[i][b]);
				}
			}

			void erase_row(size_t a){
				if ( a >= row ) throw std::invalid_argument("Row index out of range");
				
				MATRIX.erase(MATRIX.begin() + a);
				row -= 1;
				mat = row * col;
			}
			
			void erase_row(size_t a, size_t b){
				if ( a >= row ||  b >= row ) throw std::invalid_argument("Row index out of range");
				
				MATRIX.erase(MATRIX.begin() + a, MATRIX.begin() + b);
				row -= b - a;
				mat = row * col;
			}

			void erase_col(size_t a){
				if ( a >= col ) throw std::invalid_argument("Column index out of range");

				for(size_t i=0; i < row; ++i){
					MATRIX[i].erase(MATRIX[i].begin() + a);
				}
				col -= 1;
				mat = row * col;
			}

			void erase_col(size_t a,  size_t b){
				if ( a >= col ||  b >= col ) throw std::invalid_argument("Column index out of range");

				for(size_t i=0; i < row; ++i){
					MATRIX[i].erase(MATRIX[i].begin() + a, MATRIX[i].begin() + b);
				}
				col -= b - a;
				mat = row * col;
			}

			void erase(size_t drow, size_t dcol){
				if ( drow >= row ) throw std::invalid_argument("Row index out of range");
				if ( dcol >= col ) throw std::invalid_argument("Column index out of range");
					
				erase_row(drow);
				erase_col(dcol);
			}


			void insert_row(size_t prow, T value = 0)
			{
				if (prow > row) throw std::invalid_argument("Matrix insert row: Row index out of range");

				MATRIX.insert(MATRIX.begin() + prow, std::vector(col, value));

				row += 1;
				mat = row * col;
			}

			template<typename Iterator>
			void insert_row(size_t prow, Iterator b1, Iterator b2)
			{
				if (std::distance(b1, b2) != col || prow > row) throw std::invalid_argument("Matrix insert row: Row index out of range");

				MATRIX.insert(MATRIX.begin() + prow, b1, b2);

				row += 1;
				mat = row * col;
			}

			template<typename cont>
			void insert_row(size_t prow, cont c)
			{
				this -> insert_row(prow, c.begin(), c.end());
			}

			void insert_col(size_t pcol, T value = 0)
			{
				if (pcol > col) throw std::invalid_argument("Matrix insert column: Column index out of range");

				for(size_t i=0; i < row; ++i) MATRIX[i].insert(MATRIX[i].begin() + pcol, value);

				col += 1;
				mat = row * col; 
			}

			template<typename Iterator>
			void insert_col(size_t pcol, Iterator b1, Iterator b2)
			{
				if (std::distance(b1, b2) != row || pcol > col) throw std::invalid_argument("Matrix insert column: Column index out of range");

				for(size_t i=0; i < row; ++i) MATRIX[i].insert(MATRIX[i].begin() + pcol, b1 + i);

				col += 1;
				mat = row * col;
			}

			template<typename cont>
			void insert_col(size_t pcol, cont c)
			{
				this -> insert_col(pcol, c.begin(), c.end());
			}

			static matrix<T> identity(size_t n){
                                matrix<T> result(n, n);
                                for(size_t i=0; i < n; ++i){
                                        result[i][i] = 1;
                                }

				return result;
                        }

			static matrix<T> ones(size_t r, size_t c){
				matrix<T> result(r, c, 1.0);
				/*for(size_t i=0; i < r; ++i){
					for(size_t j=0; j < c; ++j){
						result[i][j] = 1;
					}
				}*/
				return result;
			}


			std::vector<T> operator()(const std::vector<T>& vec) const {
				if (vec.size() != col) throw std::invalid_argument("Size of vector and number of columns from matrix not eqvel!");
				
				std::vector<T> result(row, 0);
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j< col; ++j){
						result[i] += MATRIX[i][j] * vec[j];
					}
				}
				return result;
			}

			matrix<T> operator()(const matrix<T>& m){
				if( col != m.size_row() ) throw std::invalid_argument("Size of column in left matrix and size of rows in right matrix not eqvel!");
				
				matrix<T> result(row, m.size_col());
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < m.size_col(); ++j){
						for(size_t k=0; k < col; ++k){
							result[i][j] += MATRIX[i][k] * m[k][j];
						}
					}
				}
				return result;
			}

			matrix<T> transposition(){
				matrix<T> result(col, row);
				for(size_t i=0; i < col; ++i){
					for(size_t j=0; j < row; ++j){
						result[i][j] = MATRIX[j][i];
					}
				}

				return result;
			}

			matrix<T> diag(){
				if(!is_square()) throw std::invalid_argument("Method_Gauss: matrix is not square!");

				size_t n = size_row();
				matrix<T> M(n, n);
				for(size_t i=0; i < n; ++i){
					M[i][i] = MATRIX[i][i];
				}

				return M;
			}

			static matrix<T> Method_Gauss(const matrix<T>& A){
				if(!A.is_square()) throw std::invalid_argument("Method_Gauss: matrix is not square!");

				size_t n = A.size_row();

				matrix<T> B = A;
				matrix<T> I = identity(n);

				for(size_t k=0; k < n; ++k){
					auto col_begin = iterator_vertical(&B, k, k);
					auto col_end = iterator_vertical(&B, n - 1, k);
					auto it  = std::max_element(col_begin, col_end);
					auto dist_in_col = std::distance(col_begin, it);
					if(dist_in_col != k) {
						B.swap_row(k, k + dist_in_col);
						I.swap_row(k, k + dist_in_col);
					}

					if(std::abs(B[k][k]) < 1e-10) throw std::invalid_argument("Method_Gauss: matrix is singular");

					T pivot = B[k][k];
					for(size_t j=0; j < B.size_col(); ++j){ // j=k
						B[k][j] /= pivot;
						I[k][j] /= pivot;
					}

					for(size_t i=k+1; i < n; ++i){
						T factor = B[i][k];
						if(std::abs(factor) > 1e-10){
							for(size_t j=k; j < n; ++j){
								B[i][j] -= factor * B[k][j];
							}
							for(size_t j=0; j < n; ++j){
								I[i][j] -= factor * I[k][j];
							}
						}
					}
				}

				for(int i=n-1; i >= 0; --i){
					for(int j=i-1; j >= 0; --j){
						T factor = B[j][i] / B[i][i];
						for(size_t k = 0; k < n; ++k){
							B[j][k] -= factor * B[i][k]; // there
							I[j][k] -= factor * I[i][k];
						}
					}
				}

				return I;

			}


			static T Method_Laplas(const matrix<T>& A){
				if (!A.is_square()) throw std::invalid_argument("Method_Laplas: matrix is not square!");	

				size_t n = A.size_row();

				if ( n == 1 ) return A[0][0];
				if ( n == 2 ) return A[0][0] * A[1][1] - A[0][1] * A[1][0];

				T DET = 0;

				for(size_t j=0; j < A.size_col(); ++j){
					matrix<T> M = A;
					M.erase(0, j);

					int cof = ((j % 2) == 0) ? 1 : -1;
					DET += cof * A[0][j] * Method_Laplas(M);
				}
				return DET;
			}


			static T Sarrus_Method(const matrix<T>& A) //It works to matrix that have size less then 4*4
			{
				if (!A.is_square()) throw std::invalid_argument("Method_Laplas: matrix is not square!");
				
				int N = A.size_row(); if (N > 3) return Method_Laplas(A);
				if (N == 1) return A[0][0];
				T sum = 0.0;
				
				for (int idx = 0; idx < N; ++idx)
				{
					T mult_plus = 1.0;
					T mult_minus = 1.0;
					for (int i = 0; i < N; ++i)
					{
						T j = (idx + i < N ? i + idx : i + idx - N);
						mult_plus *= A[j][i];
						mult_minus *= A[j][N - i - 1];
					}
					sum += mult_plus - mult_minus;
				}


				return sum;
			}




			T det(){
				return Method_Laplas(*this);
			}

			
			 T algadd(size_t a, size_t b){
				if ( a >= row ) throw std::invalid_argument("Row index out of range");
				if ( b >= col ) throw std::invalid_argument("Column index out of range");

				matrix<T> A = *this;
				A.erase(a, b);

				T result = A.det();
				//if ( (a + b + 2) % 2 != 0 ) result *= -1;
				if ( (a + b) % 2 == 1 ) result *= -1;
				
				return result;
			}

			matrix<T> algadd(){
				matrix<T> A(row, col);
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						A[i][j] = algadd(i, j);
					}
				}
				
				return A;
			}

			matrix<T> attached(){
				return this->algadd().transposition();
			}
			
			matrix<T> inverse(){
				if(!is_square()) throw std::logic_error("Inverse: matrix is not square!");
				if(size() <= 9) return (1 / det()) * attached();
				else return Method_Gauss(*this);
			}


			T get_value(){
				if(mat != 1) throw std::logic_error("Class matrix, get_value - matrix not scalar!");
				return MATRIX[0][0];
			}

			
			matrix<T> cov(){
				
				matrix<T> A = *this;

				matrix<T> cov(A.size_col(), A.size_col());

				for(size_t j=0; j < A.size_col(); ++j){
					T mean = 0;
					for(size_t i=0; i < A.size_row(); ++i){
						mean += A[i][j];
					}
					mean /= A.size_row();
					for(size_t i=0; i < A.size_row(); ++i){
						A[i][j] -= mean;
					}
				}

				for(size_t j=0; j < A.size_col(); ++j){
					for(size_t k=0; k < A.size_col(); ++k){
						T mean = 0;
						for(size_t i=0; i < A.size_row(); ++i){
							mean += A[i][j] * A[i][k];
						}
						mean /=  A.size_row();
						cov[k][j] = mean;
					}
				}
				return cov;
			}


			matrix<T> cor(){

				matrix<T> A = *this;

				size_t n = A.size_col();
				matrix<T> cov = A.cov();
				matrix<T> cor = cov;

				for(size_t i=0; i<n; ++i){
					for(size_t j=0; j<n; ++j){
						cor[j][i] /= std::sqrt(cov[i][i]) * std::sqrt(cov[j][j]); 
					}
				}
				return cor;
			}

	};


	//===========================Operators_to_Matrix========================================

	template<class T1,class T2>
        auto operator+(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() + std::declval<T2>()) >{
		using ResultType = decltype(std::declval<T1>() + std::declval<T2>());

		if ( a.size_col() != b.size_col() ) throw std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size_row() != b.size_row() ) throw std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] += b[i][j];
			}
		}
	
		return result;
	}


	template<class T, class Scalar>
        auto operator+(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >{
		using ResultType = decltype(std::declval<T>() + std::declval<Scalar>());

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] += b;
			}
		}

		return result;
	}

	template<class T, class Scalar>
        auto operator+(Scalar b ,const matrix<T>& a) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >{
		return a + b;
	}

	template<class T, class Scalar>
	auto operator+=(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >
	{ return a + b; }

	template<class T1,class T2>
	auto operator+=(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() + std::declval<T2>()) >
	{ return a + b; }


	template<class T1,class T2>
        auto operator-(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() - std::declval<T2>()) >{
		using ResultType = decltype(std::declval<T1>() - std::declval<T2>());

		if ( a.size_col() != b.size_col() ) throw std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size_row() != b.size_row() ) throw std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] -= b[i][j];
			}
		}

		return result;
	}


	template<class T, class Scalar>
        auto operator-(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() - std::declval<Scalar>()) >{
		using ResultType = decltype(std::declval<T>() - std::declval<Scalar>());

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] -= b;
			}
		}

		return result;
	}


	template<class T, class Scalar>
        auto  operator-(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() - std::declval<T>()) >{
		using ResultType = decltype(std::declval<Scalar>() - std::declval<T>());

		matrix<ResultType> result(a.size_row(), a.size_col());
		for(size_t i=0; i < a.size_row(); ++i){
			for(size_t j=0; j < a.size_col(); ++j){
				result[i][j] = b - a[i][j];
			}
		}
		return result;
	}

	template<class T, class Scalar>
	auto operator-=(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() - std::declval<Scalar>()) >
	{ return a - b; }

	template<class T1, class T2>
	auto operator-=(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() - std::declval<T2>()) >
	{ return a - b; }


	template<class T, class Scalar>
        auto  operator*(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() * std::declval<T>()) >{
		using ResultType = decltype(std::declval<Scalar>() * std::declval<T>());

		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] *= b;
			}
		}

		return result;
	}

	template<class T, class Scalar> 
        auto operator*(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() * std::declval<Scalar>()) >{
		return b * a;
	}

	template<class T, class Scalar>
	auto operator*=(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() * std::declval<Scalar>()) >
	{ return b * a; }

	template<class T, class Scalar>
	auto operator/(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() / std::declval<Scalar>()) >
	{
		//using ResultType = decltype(std::declval<T>() / std::declval<Scalar>());

		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i) for(size_t j=0; j <  a.size_col(); ++j) result[i][j] /= b;

		return result;
	}

	template<class T, class Scalar>
	auto operator/=(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() / std::declval<Scalar>()) >
	{ return a / b; }
	

	template<class T>
	std::ostream& operator<<(std::ostream& os, const matrix<T>& m){
		/*if(!m.LABELS[0].empty()) os << "\t";
		if(!m.LABELS[1].empty()){
			for(auto name : m.LABELS[1]){
				os << name << "\t";
			}
		}*/
		os << "\n";
		for(size_t i=0; i < m.size_row(); ++i){
			//if(!m.LABELS[0].empty()) os << m.LABELS[0][i] << "\t";
			for(size_t j=0; j < m.size_col(); ++j){
				os << m.MATRIX[i][j] << "\t";
			}
			os << "\n";
		}
		return os;
	}

	template<class T> 
        matrix<T>& operator>>(std::istream& is, matrix<T>& m){
		std::string line;
		std::vector<std::string> date;

		while(std::getline(is, line)){
			date.push_back(line);
		}

		auto split = [&] (const std::string& str, const std::string& delimiter) {
			std::vector<std::string> tokens;
			size_t start = 0;
			size_t end;

			while ( (end = str.find(delimiter, start)) != std::string::npos){
				std::string token = str.substr(start, end - start);
				if(!token.empty()) tokens.push_back(token);
				start = end + delimiter.length();
			}

			std::string last = str.substr(start);
			if(!last.empty()) tokens.push_back(last);

			return tokens;
		};

		auto string_to = [&] (const std::string& str) {
			std::istringstream iss(str);
			T value;
			if(!(iss >> value)) throw std::invalid_argument("Read matrix: Cannot convert to type T");
			return value;
		};

		std::vector<std::string> first_line = split(date[0], "\t");
		
		size_t cols = first_line.size();
		size_t rows = date.size();
		m.resize(rows, cols);

		auto st_to_matrix = [&m, cols] (std::vector<std::string> st, size_t r) {
			if(st.size() != cols) throw std::out_of_range("Read matrix: String has got uncorrect number of column!\n");

			for(size_t col_idx=0; col_idx < cols; ++col_idx){
				try {
					T value = string_to(st[col_idx]);
					m.MATRIX[r][col_idx] = value;
				} catch (const std::exception& e) {
					std::cerr << "Mistake in row " << r + 1 << " column " << col_idx + 1 << " : '" << st[col_idx] << "' ---> " << e.what() << "\n";
				}
			}
		};

		st_to_matrix(first_line, 0);

		for(size_t row_idx=1; row_idx < rows; ++row_idx){
			std::vector<std::string> next_line = split(date[row_idx], "\t");
			if(next_line.size() >= cols) st_to_matrix(next_line, row_idx);
		}
		return m;
	}

	//===========================Iterators_to_Matrix========================================
	template<class T>
        class matrix<T>::iterator_horizontal {
        private:
                matrix<T>* M;
                size_t index;
		
		size_t calculate_index(size_t r, size_t c) {
			return c + r * M->size_col();
		}

                void calculate_position(size_t idx, size_t& r, size_t& c) {
			c = idx % M -> size_col();
			r = idx / M -> size_col();
		}

		bool is_valid_position(size_t r, size_t c){
			return r < M->size_row() && c < M->size_col();
		}

        public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = T*;
                using reference = T&;

		explicit iterator_horizontal(matrix<T>* m, size_t idx = 0) : M(m), index(idx) {
                        if (index > M->size()) index = M->size();
                }

		iterator_horizontal(matrix<T>* m, size_t r, size_t c) : M(m) {
                        if (is_valid_position(r, c)) index = calculate_index(r, c);
                        else {
				index = M->size();
				throw std::invalid_argument("iterator_vertical - index out of range");
			}
                }
		

		reference operator*() {
			size_t r, c;
			calculate_position(index, r, c);
			if (!is_valid_position(r, c)) throw std::out_of_range("iterator_vertical: dereferencing out of range!");
			return (*M)[r][c];
                }

		pointer operator->() {
			return &(*(*this));
		}

                iterator_horizontal& operator++() { ++index; return *this; }
                iterator_horizontal operator++(int) { iterator_horizontal tmp = *this; ++index; return tmp; }
                iterator_horizontal& operator--() { --index; return *this; }
                iterator_horizontal operator--(int)  { iterator_horizontal tmp = *this; --index; return tmp; }

                iterator_horizontal& operator+=(difference_type n) { index += n; return *this; }
                iterator_horizontal& operator-=(difference_type n) { index -= n; return *this; }

                friend iterator_horizontal operator+(iterator_horizontal it, difference_type n) { return iterator_horizontal(it.M, it.index + n); }
                friend iterator_horizontal operator+(difference_type n, iterator_horizontal it) { return it + n; }
                friend iterator_horizontal operator-(iterator_horizontal it, difference_type n) { return iterator_horizontal(it.M, it.index - n); }
                friend difference_type operator-(const iterator_horizontal& a, const iterator_horizontal& b) { return a.index - b.index;  }

                reference operator[](difference_type n) const { return *(*this + n); }

                bool operator==(const iterator_horizontal& other) const { return index == other.index && M == other.M; }
                bool operator!=(const iterator_horizontal& other) const { return !(*this == other); }
                bool operator<(const iterator_horizontal& other) const { return index < other.index; }
                bool operator>(const iterator_horizontal& other) const { return index > other.index; }
                bool operator<=(const iterator_horizontal& other) const { return index <= other.index; }
                bool operator>=(const iterator_horizontal& other) const { return index >= other.index; }
        };

	template<class T>
        typename matrix<T>::iterator_horizontal matrix<T>::h_begin() { return iterator_horizontal(this, 0); }

        template<class T>
        typename matrix<T>::iterator_horizontal matrix<T>::h_end() { return iterator_horizontal(this, this -> size()); }
	


	template<class T>
	class matrix<T>::iterator_vertical {
	private:
		matrix<T>* M;
		size_t index;

		size_t calculate_index(size_t r, size_t c) { 
			return r + c * M->size_row(); 
		}

		void calculate_position(size_t idx, size_t& r, size_t& c) { 
			r = idx % M -> size_row();
			c = idx / M -> size_row();
		}

		bool is_valid_position(size_t r, size_t c){
			return r < M->size_row() && c < M->size_col();
		}

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		explicit iterator_vertical(matrix<T>* m, size_t idx = 0) : M(m), index(idx) { 
			if (index > M->size()) index = M->size();
		}

		iterator_vertical(matrix<T>* m, size_t r, size_t c) : M(m) { 
			if (is_valid_position(r, c)) index = calculate_index(r, c);
			else {
				index = M->size();
				throw std::invalid_argument("iterator_vertical - index out of range");
			}
		}

		reference operator*() {
			size_t r, c;
			calculate_position(index, r, c);
			if (!is_valid_position(r, c)) throw std::out_of_range("iterator_vertical: dereferencing out of range!");
			return (*M)[r][c];
		}

		pointer operator->() {
			return &(*(*this));
		}

		iterator_vertical& operator++() { ++index; return *this; }
		iterator_vertical operator++(int) { iterator_vertical tmp = *this; ++index; return tmp; } 
		iterator_vertical& operator--() { --index; return *this; }
		iterator_vertical operator--(int) { iterator_vertical tmp = *this; --index; return tmp; } 

		iterator_vertical& operator+=(difference_type n) { index += n; return *this; }
		iterator_vertical& operator-=(difference_type n) { index -= n; return *this; }
		
		friend iterator_vertical operator+(iterator_vertical it, difference_type n) { return iterator_vertical(it.M, it.index + n); }
		friend iterator_vertical operator+(difference_type n, iterator_vertical it) { return it + n; }
		friend iterator_vertical operator-(iterator_vertical it, difference_type n) { return iterator_vertical(it.M, it.index - n); }
		friend difference_type operator-(const iterator_vertical& a, const iterator_vertical& b) { return a.index - b.index;  }

		reference operator[](difference_type n) const { return *(*this + n); }
		
		bool operator==(const iterator_vertical& other) const { return index == other.index && M == other.M; }
                bool operator!=(const iterator_vertical& other) const { return !(*this == other); }
                bool operator<(const iterator_vertical& other) const { return index < other.index; }
                bool operator>(const iterator_vertical& other) const { return index > other.index; }
                bool operator<=(const iterator_vertical& other) const { return index <= other.index; }
                bool operator>=(const iterator_vertical& other) const { return index >= other.index; }
	
	};
	
	template<class T>
        typename matrix<T>::iterator_vertical matrix<T>::v_begin() { return iterator_vertical(this, 0); }

	template<class T>
	typename matrix<T>::iterator_vertical matrix<T>::v_end() { return iterator_vertical(this, *this -> size()); }


}

//===============================================================================Applay_function_modul=========================================================================================
namespace leo{
	template<typename T>
	T string_to(const std::string& str) {
		std::istringstream iss(str);
		T value;
		if(!(iss >> value)){
			std::cerr << "Error value: " << str << "\n";
			throw std::invalid_argument("Read convert: Cannot convert to type T");
		}
		return value;
	};

	inline std::vector<std::string> split(const std::string& str, const std::string& delimiter="\t"){
		std::vector<std::string> tokens;
		size_t start = 0;
		size_t end;

		while ((end = str.find(delimiter, start)) != std::string::npos){
			std::string token = str.substr(start, end - start);
			if(!token.empty()) tokens.push_back(token);
			start = end + delimiter.length();
		}

		std::string last = str.substr(start);
		if(!last.empty()) tokens.push_back(last);

		return tokens;
	}

	

	inline std::stringstream ReadFile(const std::string& FileName){
		std::ifstream file(FileName);
		//file.open(FileName);

		if(!file.is_open()) throw std::invalid_argument("ReadFile: error of reading file!\n");

		std::stringstream F;
		F << file.rdbuf();
		//file.close();
		return F;
	}


	inline void WriteFile(const std::string& FileName, std::istream& is){
		std::ofstream file(FileName);
		if(!file.is_open()) throw std::invalid_argument("WriteFile: error of open/creat output file!\n");

		file << is.rdbuf();
		//file.close();
	}

}

//===============================================================================Math_modul=========================================================================================
namespace leo{
	const double pi = M_PI;

	template<class Iterator>
	auto MathExcept(Iterator start, Iterator end){

		using ValueType = typename std::iterator_traits<Iterator>::value_type;

		if(start == end) throw std::invalid_argument("MathEx: empty datum!");

		ValueType sum = std::accumulate(start, end, ValueType{});

		auto n = std::distance(start, end);

		return static_cast<double>(sum) / n;
	}


	template<typename T>
	auto MathExcept(std::vector<T> X){	return MathExcept(X.begin(), X.end());	}


	template<class Iterator>
	auto RMS(Iterator start, Iterator end){
		if(start == end) throw std::invalid_argument("RMS: empty datum!");
	
		double result = std::inner_product(start, end, start, 0.0);

		result /= std::distance(start, end);
		
		return std::sqrt(result);
		
	}


	template<typename T>
	auto RMS(std::vector<T> X){	return RMS(X.begin(), X.end());	}



	template<class Iterator>
	auto Variation(Iterator start, Iterator end){
		if(start == end) throw std::invalid_argument("Variation: empty datum!");
		
		auto n = std::distance(start, end);

		auto M = MathExcept(start, end);

		std::vector<double> res;
		res.reserve(n);
		
		std::transform(start, end, std::back_inserter(res), [M] (double x) { return x - M; });

		return RMS(res.begin(), res.end());
	}


	template<typename T>
	auto Variation(std::vector<T> X){	return Variation(X.begin(), X.end());	}


	template<class Iterator1, class Iterator2>
	auto CCF(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, int tau, bool norm=true){
		if(fb == fe || gb == ge) return 0.0;
		auto sum = 0.0;
		auto Mf = MathExcept(fb, fe);
		auto Mg = MathExcept(gb, ge);
		auto f_dist = std::distance(fb, fe);
		auto g_dist = std::distance(gb, ge);
		int tau_f = 0;
		int tau_g = 0;
		int top;
		if(tau >= 0) {
			tau_f = tau;
			top = std::min(f_dist - 1 - tau_f, g_dist - 1);
		} else {
			tau_g = -tau;
			top = std::min(g_dist - 1 - tau_g, f_dist - 1);
		}
		if(top < 0) return 0.0;
		for(size_t i=0; i <= top; ++i){
			auto f_el = *(fb + i + tau_f);
			auto g_el = *(gb  + i + tau_g);
			if(norm) { f_el -= Mf; g_el -= Mg; }
			sum += f_el * g_el;
		}
		if(norm && top != 0) return sum/top;
		return sum;
	}




	template<typename T1, typename T2>
	auto CCF(std::vector<T1> f, std::vector<T2> g, int tau,  bool norm=true){	return  CCF(f.begin(), f.end(), g.begin(), g.end(), tau, norm);	}


	template<class Iterator1, class Iterator2>
	std::vector<double> CCF(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, bool norm=true, bool natural=false){
		auto f_dist = std::distance(fb, fe);
		auto g_dist = std::distance(gb, ge);
		std::vector<double> result;
		if(natural) result.reserve(f_dist);
		else result.reserve(f_dist + g_dist - 1);
		int start_lag = -(g_dist-1);
		int end_lag = (f_dist - 1);
		if(natural) start_lag = 0;
		for(int lag = start_lag; lag <= end_lag; ++lag){
			result.emplace_back(CCF(fb, fe, gb, ge, lag, norm));
		}
		return result;
	}



	template<typename T1, typename T2>
	std::vector<double> CCF(std::vector<T1> f, std::vector<T2> g, bool norm=false, bool natural=false){	return  CCF(f.begin(), f.end(), g.begin(), g.end(), norm, natural);	}

	
	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe, int tau,  bool norm=true){
		return CCF(fb, fe, fb, fe, tau, norm);
	}


	template<typename T1>
	auto ACF(std::vector<T1> f, int tau,  bool norm=true) {	return CCF(f.begin(), f.end(), f.begin(), f.end(), tau, norm);	}



	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe,  bool norm=true) -> std::vector<typename std::iterator_traits<Iterator>::value_type>
	{
		using T = typename std::iterator_traits<Iterator>::value_type;

		auto f_dist = std::distance(fb, fe);
		std::vector<T> result;
		result.reserve(f_dist);
		for(size_t lag=0; lag < f_dist; ++lag){
			result.emplace_back(ACF(fb, fe, lag, norm));
		}
		return result;
	}


	template<typename T1>
	auto ACF(std::vector<T1> f,  bool norm=true) {	return ACF(f.begin(), f.end(), norm);	}



namespace Matrix{
	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe,  bool norm=true) -> matrix<typename std::iterator_traits<Iterator>::value_type> 
	{
		using T = typename std::iterator_traits<Iterator>::value_type;
		auto f_dist = std::distance(fb, fe);
		matrix<T> A(f_dist, f_dist);
		for(int i=0; i!=f_dist; ++i){
			for(int j=0; j!=f_dist; ++j){
				int lag = std::abs(j - i);
				A[i][j] = leo::ACF(fb, fe, lag, norm);
			}
		}
	
		return A;
	}

	template<typename T1>
	auto ACF(std::vector<T1> f,  bool norm=true) {   return Matrix::ACF(f.begin(), f.end(), norm);	}

	template<class T>
	matrix<T> Cov(matrix<T> A){
		int col = A.size_col(); 
		int row = A.size_row();

		matrix<T> M(1, col);

		for(size_t i=0; i < col; ++i){
			auto start = A.Column(i).begin();
			auto end = A.Column(i).end();
			auto m = MathExcept(start, end);
			M[0][i] = m;
		}
		
		return 1.0 / (row - 1.0) * (A.transposition()(A) - row * M.transposition()(M));
	}

	template<class T>
	matrix<T> Cor(const matrix<T>& A){
		matrix<T> covariation = Matrix::Cov(A);
		matrix<T> disp = covariation.diag();
		for(size_t i=0; i < disp.size_col(); ++i) disp[i][i] = std::sqrt(disp[i][i]);
		matrix<T> invd = disp.inverse();
		return invd(covariation(invd));
	}
}
	
	template<class T>
	std::vector<T> solve(matrix<T> A, std::vector<T> d){
		if(A.size_row() < A.size_col()) {
			matrix<T> A_t = A.transposition();
			return A_t( (A(A_t)).inverse()(d) );
		} else if(A.size_row() > A.size_col()){ 
			matrix<T> A_t = A.transposition();
			return ( A_t(A) ).inverse()( A_t(d) );
		} else return A.inverse()(d);
	}




namespace BackFFT{

	template<class T>
	void FFT(std::vector<std::complex<T>>& x, bool inverse=false){
		const size_t N = x.size();
		if(N<=1) return;

		std::vector<std::complex<T>> even(N/2);
		std::vector<std::complex<T>> odd(N/2);

		for(size_t i=0; i < N/2; ++i){
			even[i] = x[i * 2];
			odd[i] = x[i * 2 + 1];
		}

		FFT(even, inverse);
		FFT(odd, inverse);

		T angle = 2 * pi / N * (inverse ? 1 : -1);
		std::complex<T> w(1);
		std::complex<T> wn(std::cos(angle), std::sin(angle));

		for(size_t i=0; i<N/2; ++i){
			x[i] = even[i] + w * odd[i];
			x[i + N/2] = even[i] - w * odd[i];
			if(inverse){
				x[i] /= 2;
				x[i + N/2] /= 2;
			}
			w *= wn;
		} 

	}

	size_t next_power_of_two(size_t n){
		if(n == 0) return 1;
		size_t power = 1;
		while (power < n) power <<= 1;
		return power;
	}

	template<typename T>
	struct is_complex : std::false_type {};

	template<typename T>
	struct is_complex<std::complex<T>> : std::true_type {};

	template<typename T>
	inline constexpr bool is_complex_v = is_complex<T>::value;

	template<typename Iterator>
	bool is_complex_iterator(Iterator it) {
		using ValueType = std::decay_t<decltype(*it)>;
		return is_complex_v<ValueType>;
	}


	//////
	template<typename T>
        struct scalar_type {
                using type = T;
        };

        template<typename T>
        struct scalar_type<std::complex<T>> {
                using type = T;
        };

        template<typename T>
        using scalar_type_t = typename scalar_type<std::decay_t<T>>::type;
	///////



	template<typename Iterator>
	auto FFT_complex(Iterator xb, Iterator xe, bool inverse=false, bool full=0)// -> std::vector<decltype(*xb)>
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		size_t n = std::distance(xb, xe);

		std::vector<std::complex<value_type>> res;
		res.reserve(n);

		for(auto it = xb; it != xe; ++it){ 
			res.emplace_back(*it);
		}

		size_t N = next_power_of_two(n);
		res.resize(N, {});

		BackFFT::FFT(res, inverse);
		if(!full) res.resize(n);
		
		return res;
	}

	template<typename Iterator>
	auto FFT_real_back(Iterator xb, Iterator xe, bool inverse=false, bool full=0)// -> std::vector<decltype(*xb)>
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		size_t n = std::distance(xb, xe);
			
		std::vector<std::complex<value_type>> res;
		res.reserve(n);

		for(auto it = xb; it != xe; ++it){
			res.emplace_back(std::complex<value_type>(*it, 0));
		}

		size_t N = next_power_of_two(n);
		res.resize(N, {});

		BackFFT::FFT(res, inverse);
		
	
		std::vector<value_type> result(N);
		for(size_t i=0; i < N; ++i){
			 result[i] = res[i].real();
		}
		if(!full) result.resize(n);

		return result;
	}


	template<typename Iterator>
	auto FFT_real_front(Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		size_t n = std::distance(xb, xe);

		std::vector<std::complex<value_type>> res;
		res.reserve(n);

		for(auto it = xb; it != xe; ++it){
			res.emplace_back(std::complex<value_type>(*it, 0));
		}

		size_t N = next_power_of_two(n);
		res.resize(N, {});

		BackFFT::FFT(res, inverse);
		if(!full) res.resize(n);
		
		return res;
	}


}

	template<typename Iterator>
	auto FFT(Iterator xb, Iterator xe, bool inverse=false, bool full=0)// -> std::vector<decltype(*xb)>
	{
		using value_type = std::decay_t<decltype(*xb)>;
		if constexpr (!BackFFT::is_complex_v<value_type>) return BackFFT::FFT_real_back(full, xb, xe, inverse);
		else {
			if(BackFFT::is_complex_v<value_type>) return BackFFT::FFT_complex(full, xb, xe, inverse);
		}
	}



	template<typename T>
	struct scalar_type {
		using type = T;
	};

	template<typename T>
	struct scalar_type<std::complex<T>> {
		using type = T;
	};

	template<typename T>
	using scalar_type_t = typename scalar_type<std::decay_t<T>>::type; 

	struct complex_tag {};
	struct real_tag {};
	struct image_tag {};


	inline constexpr complex_tag complex{};
	inline constexpr real_tag real{};
	inline constexpr image_tag image {};

	template<typename Iterator>
	auto FFT(real_tag, Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;
		
		std::vector<std::complex<value_type>> result;
		
		if constexpr (BackFFT::is_complex_v<elem_type>) result = BackFFT::FFT_complex(xb, xe, inverse, full);
		else result = BackFFT::FFT_real_front(xb, xe, inverse, full);

		std::vector<value_type> real_part;
		real_part.reserve(result.size());
		for(const auto& x : result){
			real_part.push_back(x.real());
		}
		return real_part;
	}


	template<typename Iterator>
	auto FFT(image_tag, Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		std::vector<std::complex<value_type>> result;

		if constexpr (BackFFT::is_complex_v<elem_type>) result = BackFFT::FFT_complex(xb, xe, inverse, full);
		else result = BackFFT::FFT_real_front(xb, xe, inverse, full);

		std::vector<value_type> image_part;
		image_part.reserve(result.size());
		for(const auto& x : result){
			image_part.push_back(x.imag());
		}
		return image_part;
	}


	template<typename Iterator>
	auto FFT(complex_tag, Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		std::vector<std::complex<value_type>> result;

		if constexpr (BackFFT::is_complex_v<elem_type>) result = BackFFT::FFT_complex(xb, xe, inverse, full);
		else result = BackFFT::FFT_real_front(xb, xe, inverse, full);

		return result;
	}
	

	template<typename Tag, typename T>
	auto FFT(Tag tag, std::vector<T> X, bool inverse=false, bool full=0){	return FFT(tag, X.begin(), X.end(), inverse, full);	}

	template<typename T>
	auto FFT(std::vector<T> X, bool inverse=false, bool full=0){	return FFT(X.begin(), X.end(), inverse, full);	}



/*	template<typename T>
	std::vector<std::complex<T>> DFT(std::vector<std::complex<T>>& x, bool inverse=false){
			int N = x.size();
			std::complex<double> X(N, {});
			for(size_t i = 0; i < N; ++i){
				for(size_t j = 0; j < N; ++j){
					X[i] += x[j] * std::exp( std::complex<T>(0, 2 * pi * i * j / N * (inverse ? -1 : 1))  );
				}
				if(inverse) X[i] /= N;
				
			}
			return X;
	}
*/

	template<typename Iterator>
	auto DFT(Iterator xb, Iterator xe, bool inverse=false);





	template<typename Iterator1, typename Iterator2>
	auto ConvolveFFT(Iterator1 Ab, Iterator1 Ae, Iterator2 Bb, Iterator2 Be)
	{
		using A_elem_type = std::decay_t<decltype(*Ab)>;
		using A_scalar_type = scalar_type_t<A_elem_type>;
		using B_elem_type = std::decay_t<decltype(*Bb)>;
		using B_scalar_type = scalar_type_t<B_elem_type>;
		using result_elem_type = decltype( std::declval<A_elem_type>() * std::declval<B_elem_type>() );
		using result_scalar_type = decltype( std::declval<A_scalar_type>() * std::declval<B_scalar_type>() );

		auto NA = std::distance(Ab, Ae);
		auto NB = std::distance(Bb, Be);
		size_t N = 1;
		size_t N_cor = NA + NB - 1;

		while(N < NA + NB - 1) N<<=1;

		std::vector<std::complex<A_scalar_type>> F1(N, 0);
		std::vector<std::complex<B_scalar_type>> F2(N, 0);

		for(size_t i=0; i < NA; ++i) F1[i] = *(Ab + i);
		for(size_t i=0; i < NB; ++i) F2[i] = *(Bb + i);

		BackFFT::FFT(F1);
		BackFFT::FFT(F2);

		for(size_t i=0; i < N; ++i) F1[i] *= F2[i];

		BackFFT::FFT(F1, true);

		F1.resize(N_cor);

		if constexpr (BackFFT::is_complex_v<result_elem_type>) return F1;
		else {
			std::vector<result_scalar_type> real_part(N_cor, 0);
			for (size_t i=0; i < N_cor; ++i) real_part[i] = F1[i].real();
			return real_part;
		}
	}



	template<typename Iterator1, typename Iterator2>
	auto Convolve(Iterator1 Ab, Iterator1 Ae, Iterator2 Bb, Iterator2 Be)
	{	
		using A_elem_type = std::decay_t<decltype(*Ab)>;
		using B_elem_type = std::decay_t<decltype(*Bb)>;
		using result_elem_type = decltype( std::declval<A_elem_type>() * std::declval<B_elem_type>() );

		size_t NA = std::distance(Ab, Ae);
		size_t NB = std::distance(Bb, Be);
		size_t N = NA + NB - 1;

		std::vector<result_elem_type> Res(N, {});

		for(auto it_a = Ab; it_a != Ae; ++it_a){
			for(auto it_b = Bb; it_b != Be; ++it_b){
				size_t i = std::distance(Ab, it_a) + std::distance(Bb, it_b);// - 2;
				Res[i] += *it_a * *it_b;
			}
		}

		return Res;
	}


	template<typename T1, typename T2>
	auto Convolve(std::vector<T1> F, std::vector<T2> G){
		return Convolve(F.begin(), F.end(), G.begin(), G.end());
	}

	template<typename T1, typename T2>
	auto ConvolveFFT(std::vector<T1> F, std::vector<T2> G){
		return ConvolveFFT(F.begin(), F.end(), G.begin(), G.end());
	}


	template<typename Iterator1, typename Iterator2>	
	auto CCFbyFFT(Iterator1 Ab, Iterator1 Ae, Iterator2 Bb, Iterator2 Be){
		using A_elem_type = std::decay_t<decltype(*Ab)>;
		using A_scalar_type = scalar_type_t<A_elem_type>;
		using B_elem_type = std::decay_t<decltype(*Bb)>;
		using B_scalar_type = scalar_type_t<B_elem_type>;
		using result_elem_type = decltype( std::declval<A_elem_type>() * std::declval<B_elem_type>() );
		using result_scalar_type = decltype( std::declval<A_scalar_type>() * std::declval<B_scalar_type>() );

		auto NA = std::distance(Ab, Ae);
		auto NB = std::distance(Bb, Be);
		size_t N = 1;
		size_t N_cor = NA + NB - 1;

		while(N < NA + NB - 1) N<<=1;

		std::vector<std::complex<A_scalar_type>> F1(N, 0);
		std::vector<std::complex<B_scalar_type>> F2(N, 0);

		for(size_t i=0; i < NA; ++i) F1[i] = *(Ab + i);
		for( int i=(NB - 1); i >= 0; --i) F2[NB - 1 - i] = *(Bb + i);

		BackFFT::FFT(F1);
		BackFFT::FFT(F2);

		for(size_t i=0; i < N; ++i) F1[i] *= F2[i];
	
		BackFFT::FFT(F1, true);

		F1.resize(N_cor);

		if constexpr (BackFFT::is_complex_v<result_elem_type>) return F1;
		else {
			std::vector<result_scalar_type> real_part(N_cor, 0);
			for (size_t i=0; i < N_cor; ++i) real_part[i] = F1[i].real();
			return real_part;
		}
		
	}


}

//===============================================================================Filters_modul=========================================================================================

namespace leo{
	

	template<typename Iterator1, typename Iterator2>
	auto Filter_Kolmogorow_Winer(Iterator1 Sb, Iterator1 Se, Iterator2 ESb, Iterator2 ESe, bool norm=false, bool filtiring=false){
		auto ccf = CCF(Sb, Se, ESb, ESe, norm, true);
		auto M = Matrix::ACF(Sb, Se, norm).inverse();
		ccf.resize(M.size_col());
		auto h_filter = M( ccf );
		if (filtiring) return ConvolveFFT(Sb, Se, h_filter.begin(), h_filter.end());
		else return h_filter;
	}


	


	template<typename T1, typename T2>
	auto Filter_Kolmogorow_Winer(std::vector<T1> signal, std::vector<T2> excepted_signal, bool norm=false, bool filtiring=false){
		return Filter_Kolmogorow_Winer(signal.begin(), signal.end(), excepted_signal.begin(), excepted_signal.end(), norm, filtiring); 
	}

	template<typename Iterator1, typename Iterator2>
	auto Filter_Consistent(Iterator1 Noise_b, Iterator1 Noise_e, Iterator2 ESb, Iterator2 ESe, bool norm=false){
		using signal_type = std::decay_t<decltype(*Noise_b)>;

		auto ccf = CCF(Noise_b, Noise_e, ESb, ESe, norm, true);
		auto M = Matrix::ACF(Noise_b, Noise_e, norm).inverse();
		M.resize(M.size_col(), ccf.size);
		auto h_filter = M( ccf );
		matrix<signal_type> S(1, h_filter.size());
		for(size_t i=0; i < h_filter.size(); ++i) S[0][i] =  h_filter[i];
		auto h_norm = S(h_filter).get_value();
		for(size_t i=0; i < h_filter.size(); ++i) h_filter[i] /= h_norm;
		return h_filter;
	}

	template<typename Iterator1, typename Iterator2, typename Iterator3>
	auto Filter_Consistent(Iterator1 Noise_b, Iterator1 Noise_e, Iterator2 ESb, Iterator2 ESe, Iterator3 Sb, Iterator3 Se, bool norm=false){
		auto h_filter = Filter_Consistent(Noise_b, Noise_e, ESb, ESe, norm);
		return ConvolveFFT(Sb, Se, h_filter.begin(), h_filter.end());
	}

	template<typename T1, typename T2>
        auto Filter_Consistent(std::vector<T1> noise, std::vector<T2> excepted_signal, bool norm=false){
		return Filter_Consistent(noise.begin(), noise.end(), excepted_signal.begin(), excepted_signal.end(), norm);
	}

	template<typename T1, typename T2, typename T3>
	auto Filter_Consistent(std::vector<T1> noise, std::vector<T2> excepted_signal, std::vector<T3> signal, bool norm=false){
		return Filter_Consistent(signal.begin(), signal.end(), excepted_signal.begin(), excepted_signal.end(), signal.begin(), signal.end(), norm);
	}

	/*template<typename Iterator1, typename Iterator2>
	auto Filter_Energy(Iterator1 Noise_b, Iterator1 Noise_e, Iterator2 ESb, Iterator2 ESe){
		using signal_type = std::decay_t<decltype(*Sb)>;

		
	}*/


}


//===============================================================================Solve_linal_modul========================================================================================= 

namespace leo{

struct Norm_FS_C_tag {};
struct Norm_FS_L1_tag {};
struct Norm_FS_L2_tag {};
struct Norm_FS_W_tag {};

inline constexpr Norm_FS_C_tag nfs_C {};
inline constexpr Norm_FS_L1_tag nfs_L1 {};
inline constexpr Norm_FS_L2_tag nfs_L2 {};
inline constexpr Norm_FS_W_tag nfs_W {};


template<typename Iterator>
auto Norm_FuncSpace_C(Iterator fb, Iterator fe) -> std::decay_t<decltype(*fb)>
{
	if( fb == fe) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");
	else return std::max(fb, fe);
}

template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_C(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	if( fb == fe || gb == ge ) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");
	
	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto rVal = std::abs( *fb - *gb );

	for(size_t i = 1; i < dist; ++i){
		auto nVal = std::abs( *(fb + i) - *(gb + i));
		if( rVal < nVal ) rVal = nVal;
	}

	return rVal;
}


template<typename Iterator>
auto Norm_FuncSpace(Norm_FS_C_tag, Iterator fb, Iterator fe)
{
	return Norm_FuncSpace_C(fb, fe);
}

template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace(Norm_FS_C_tag, Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	return Norm_FuncSpace_C(fb, fe, gb, ge);
}


template<typename Iterator>
auto Norm_FuncSpace_L1(Iterator fb, Iterator fe)
{
	if( fb == fe) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");

	auto f_dist = std::distance(fb, fe);

	auto rVal = std::abs(*fb);

	for(size_t i = 1; i < f_dist; ++i){
		rVal += std::abs(*(fb + i));
	}

	return rVal;
}



template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_L1(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	if( fb == fe || gb == ge ) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");

	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto rVal = std::abs( *fb - *gb );

	for(size_t i = 1; i < dist; ++i){
		rVal += std::abs(*(fb + i) - *(gb + i));
	}

	return rVal;
}

template<typename Iterator>
auto Norm_FuncSpace(Norm_FS_L1_tag, Iterator fb, Iterator fe)
{
	return Norm_FuncSpace_L1(fb, fe);
}

template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace(Norm_FS_L1_tag, Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	return Norm_FuncSpace_L1(fb, fe, gb, ge);
}


template<typename Iterator>
auto Norm_FuncSpace_L2(Iterator fb, Iterator fe)
{
	if( fb == fe) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");

	auto f_dist = std::distance(fb, fe);

	auto rVal = *fb * *fb;

	for(size_t i = 1; i < f_dist; ++i){
		rVal += *(fb + i) * *(fb + i);
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_L2(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	if( fb == fe || gb == ge ) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");

	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto rVal = (*fb - *gb) * (*fb - *gb);

	for(size_t i = 1; i < dist; ++i){
		rVal += (*(fb + i) - *(gb + i)) * (*(fb + i) - *(gb + i));
	}

	rVal = std::sqrt(rVal);

	return rVal;
}

template<typename Iterator>
auto Norm_FuncSpace(Norm_FS_L2_tag, Iterator fb, Iterator fe)
{
	return Norm_FuncSpace_L2(fb, fe);
}


template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace(Norm_FS_L2_tag, Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	return Norm_FuncSpace_L2(fb, fe, gb, ge);
}



template<typename Iterator>
auto Norm_FuncSpace_W(Iterator fb, Iterator fe, double g=1.0, double w_d=1.0)
{
	if( fb == fe) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");

	auto f_dist = std::distance(fb, fe);

	if(f_dist < 3) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massiver less them 3");

	auto f_d = (*(fb + 2)  - *fb)/(2 * w_d);

	auto rVal = *(fb + 1) * *(fb + 1) + g * g * f_d * f_d;

	if(f_dist == 3) return std::sqrt(rVal);

	for(size_t i = 2; i < (f_dist - 1); ++i){
		f_d = (*(fb + i + 1)  - *(fb + i - 1))/(2 * w_d);
		rVal += *(fb + i) * *(fb + i) + g * g * f_d * f_d;
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_W(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, double g=1.0, double wf_d=1.0, double wg_d=1.0)
{
	if( fb == fe || gb == ge ) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 1");

	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	if(dist < 3) throw std::invalid_argument("Norm_FuncSpace_W: Lenght of massive less them 3");

	auto f_d = (*(fb + 2) - *fb)/(2 * wf_d);
	auto g_d = (*(gb + 2) - *gb)/(2 * wg_d);

	auto fg_d = *(fb + 1) - *(gb + 1);

	auto rVal = fg_d * fg_d + g * g * (f_d - g_d) * (f_d - g_d);

	if(dist == 3) return std::sqrt(rVal);

	for(size_t i = 2; i < (dist - 1); ++i){
		f_d = (*(fb + i + 1) - *(fb + i - 1))/(2 * wf_d);
		g_d = (*(gb + i + 1) - *(gb + i - 1))/(2 * wg_d);
		fg_d = *(fb + i) - *(gb + i);

		rVal += fg_d * fg_d + g * g * (f_d - g_d) * (f_d - g_d);
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


template<typename Iterator>
auto Norm_FuncSpace(Norm_FS_W_tag, Iterator fb, Iterator fe, double g=1.0, double w_d=1.0)
{
	return Norm_FuncSpace_W(fb, fe, g, w_d);
}

template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace(Norm_FS_W_tag, Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, double g=1.0, double wf_d=1.0, double wg_d=1.0)
{
	return Norm_FuncSpace_W(fb, fe, gb, ge, g, wf_d, wg_d);
}


template<typename Iterator1, typename Iterator2>
auto Frechet_derivative(Iterator1 Func_begin, Iterator1 Func_end, Iterator2 value1_begin, Iterator2 value2_end)
{
	auto Func_dist = std::distance(Func_begin, Func_end);
	auto value1_dist = std::distance(value1_begin, value2_end);

	if(Func_dist != value1_dist || Func_dist < 3) throw std::invalid_argument("Frechet_derivative: Lenghts of Fuunction and Argument not eqval");

	using Func_tupe = std::decay_t< decltype(*Func_begin) >;
	using value1_type = std::decay_t< decltype(*value1_begin) >;

	using result_tupe = decltype( std::declval<Func_tupe> / std::declval<value1_type> );

	size_t N = Func_dist;

	std::vector<result_tupe> result(N, 0);

	for(size_t i=0; i < N; ++i){
		if(i == 0) result[i] = (*(Func_begin + i + 1) - *(Func_begin + i) ) / ( *(value1_begin + i + 1) - *(value1_begin + i));
		else if(i == N) result[i] = (*(Func_begin + i) - *(Func_begin + i - 1) ) / ( *(value1_begin + i) - *(value1_begin + i - 1));
		else result[i] = (*(Func_begin + i + 1) - *(Func_begin + i - 1) ) / ( *(value1_begin + i + 1) - *(value1_begin + i - 1));      
	}

	return result;
}

template<typename T1, typename T2>
auto Frechet_derivative(T1 Func, T2 Value){
	return Frechet_derivative(Func.begin(), Func.end(), Value.begin(), Value.end());
}


template<typename Iterator1, typename T>
auto Frechet_derivative(Iterator1 Func_begin, Iterator1 Func_end, matrix<T> M)
{
	auto Func_dist = std::distance(Func_begin, Func_end);

	if(Func_dist != M.size_row()) throw std::invalid_argument("Frechet_derivative: Lenghts of Function and size row of Matrix not eqval! ");

	size_t N_row = M.size_row();

	size_t N_col = M.size_col();

	matrix<T> result(N_row, N_col);

	for(size_t i=0; i < N_col; ++i){
		for(size_t j=0; j < N_row; ++j){
			if(j == 0) result[i][j] = (*(Func_begin + j + 1) - *(Func_begin + j - 1) ) / ( M[i][j + 1] - M[i][j]);
			else if (j == (N_col - 1)) result[i][j] = (*(Func_begin + j) - *(Func_begin + j - 1) ) / ( M[i][j] - M[i][j - 1]);
			else result[i][j] = (*(Func_begin + j + 1) - *(Func_begin + j - 1) ) / ( M[i][j + 1] - M[i][j - 1]);
		}
	}

	return result;
}

template<typename T1, typename T2>
auto Frechet_derivative(T1 Func, matrix<T2> M){
	return Frechet_derivative(Func.begin(), Func.end(), M);
}


template<typename Iterator1, typename T>
auto Hessian(Iterator1 Func_begin, Iterator1 Func_end, matrix<T> M)
{
	auto Func_dist = std::distance(Func_begin, Func_end);

	if(Func_dist != M.size_row()) throw std::invalid_argument("Hessian: Lenghts of Function and size row of Matrix not eqval! ");

	matrix<T> derivative = Frechet_derivative(Func_begin, Func_end, M);

	std::vector<matrix<T>> Hes(Func_dist);

	size_t N = M.size_col();

	for(size_t m=0; m < Func_dist; ++m){
		Hes[m].resize(N, N);

		for(size_t i=0; i < N; ++i){
			for(size_t j=0; j < N; ++j){
				if(m < 1) Hes[m][i][j] = (derivative[m + 1][j] - derivative[m][j]) / (M[m + 1][i] - M[m][i]);
				else if(m >= N - 1) Hes[m][i][j] = (derivative[m][j] - derivative[m - 1][j]) / (M[m][i] - M[m - 1][i]);
				else Hes[m][i][j] = (derivative[m + 1][j] - derivative[m - 1][j]) / (M[m + 1][i] - M[m - 1][i]);
			}
		}
	}

	return Hes;
}


template<typename Iterator1, typename Iterator2>
auto Hessian(Iterator1 Func_begin, Iterator1 Func_end, Iterator2 value1_begin, Iterator2 value2_end)
{
	auto H = Frechet_derivative(Func_begin, Func_end, value1_begin, value2_end);
	return Frechet_derivative(H.begin(), H.end(), value1_begin, value2_end);
}

}

//===============================================================================Class_LUdecomposition=========================================================================================

namespace leo
{
	template<typename T>
	class LUdecomposition
	{
		private:
			int Size = 0;
			int detP = 1;
			std::vector<size_t> perm;
			matrix<T> L;
			matrix<T> U;
			matrix<T> A;
		public:
			LUdecomposition(){};

			void decLU(matrix<T> B);
			
			LUdecomposition(matrix<T> B) { decLU(B); };

			matrix<T> getL() { return L; };
			matrix<T> getU() { return U; };
			matrix<T> getA() { return A; };
			int getDetP() { return detP; };
			int size() { return Size * Size; };
			int square_size() { return Size; };

			T det();

			T algadd(size_t , size_t b);

			matrix<T> algadd();
	
			template<typename Iterator>
			vector<T> solve(Iterator B_begin, Iterator B_end);

			template<typename cont>
			vector<T> solve(cont B);

			matrix<T> inverse();		
	};

	template<typename T>
	void LUdecomposition<T>::decLU(matrix<T> B)
	{
		if (!B.is_square()) throw std::invalid_argument("LU decomposition: matrix is not square!");
		
		int n = B.size_row();

		this -> L.resize(n, n);
		this -> A = B;
		this -> Size = n;

		this -> perm.clear();
		this -> perm.resize(n);
		std::iota(this -> perm.begin(), this -> perm.end(), 0);		

		for(size_t k=0; k < n; ++k)
		{
			auto col_begin = typename matrix<T>::iterator_vertical(&B, k, k);
			auto col_end = typename matrix<T>::iterator_vertical(&B, n - 1, k);
			auto it  = std::max_element(col_begin, col_end);
			auto dist_in_col = std::distance(col_begin, it);
			if(dist_in_col != k) 
			{
				B.swap_row(k, k + dist_in_col);
				this -> L.swap_row(k, k + dist_in_col);
				this -> detP *= -1;
				std::swap(this -> perm[k], this -> perm[k + dist_in_col]);
			}

			if(std::abs(B[k][k]) < 1e-15) throw std::invalid_argument("LU decomposition: matrix is singular");

			T pivot = B[k][k];
			this -> L[k][k] = 1;

			for(size_t i=k+1; i < n; ++i)
			{
				T factor = B[i][k] / pivot;
				this -> L[i][k] = factor;
				B[i][k] = 0;
				if(std::abs(factor) > 1e-10) 
				{
					for(size_t j=k + 1; j < n; ++j)
					{
						B[i][j] -= factor * B[k][j];
					}
				}
			}
		}

		this -> U = B;
	}


	template<typename T>
	T LUdecomposition<T>::det()
	{
		T mult = 1;
		int N = this -> Size;
		if (!N) throw std::invalid_argument("LU decomposition: decomposition is empty! (No init)");

		for (int i = 0; i < N; ++i)
		{
			mult *= this -> U[i][i];
		}

		mult *= this -> detP;

		return mult;
	}

	
	template<typename T>
	template<typename Iterator>
	vector<T> LUdecomposition<T>::solve(Iterator B_begin, Iterator B_end)
	{
		auto N = std::distance(B_begin, B_end);
		if (!this -> Size) throw std::invalid_argument("LU decomposition ::solve: decomposition is empty! (No init)");
		if (!N || N != this -> Size) throw std::invalid_argument("LU decomposition ::solve: invalid lenght of input vector!");

		vector<T> Y;
		Y.resize(N);
		for (int i = 0; i < N; ++i) 
		{
			Y[this -> perm[i]] = *B_begin;
			B_begin++;
		}

		for (int i = 1; i < N; ++i)
		{
			T sum = 0;

			for (int j = 0; j < i; ++j)
			{
				sum += this -> L[i][j] * Y[j];
			}
			Y[i] -= sum;
		}

		vector<T> X(N, 0);
		X[N - 1] = Y[N - 1] / this -> U[N - 1][N - 1];

		for (int i = N - 2; i >= 0; --i)
		{
			T sum = 0;
			for (int j = i + 1; j < N; ++j)
			{
				sum += this -> U[i][j] * X[j];
			}
	
			X[i] = 1 / (this -> U[i][i]) * (Y[i] - sum);
		}

		return X;
	}

	template<typename T>
	template<typename cont>
	vector<T> LUdecomposition<T>::solve(cont B) 
	{ return this -> solve(B.begin(), B.end());}


	template<typename T>
	matrix<T> LUdecomposition<T>::inverse()
	{
		int N = this -> Size;
		if (!N) throw std::invalid_argument("LU decomposition ::inverse: decomposition is empty! (No init)");

		matrix<T> A_inv(N, N);
		matrix<T> E = matrix<T>::identity(N);

		std::vector<std::thread> t;
		for (int i = 0; i < N; ++i) t.push_back( std::thread( [this, &E, &A_inv, i]
		{
			vector<T> X = this -> solve(E.Column(i).begin(), E.Column(i).end());
			std::copy(X.begin(), X.end(), A_inv.Column(i).begin());
		}
		));

		
		for (int i = 0; i < N; ++i) t[i].join();

		return A_inv;
	}


	template<typename T>
	T LUdecomposition<T>::algadd(size_t a, size_t b)
	{
		int N = this -> Size;
		if (!N) throw std::invalid_argument("LU decomposition ::algadd: decomposition is empty! (No init)");

		matrix<T> At = this -> A;
		LUdecomposition<T> lu(At.erase(a, b));
		T result = lu.det() * ( (a + b) % 2 == 1 ? -1 : 1 );

		return result;
	}

	template<typename T>
	matrix<T> LUdecomposition<T>::algadd()
	{
		int N = this -> Size;

		matrix<T> B;
		std::vector<std::thread> t;
		
		for (size_t i = 0; i < N; ++i) for (size_t j = 0; j < N; ++j) t.push_back( std::thread([this, &B, i, j]{ B[i][j] = this -> algadd(i, j);}) );
		for (int i = 0; i < N * N; ++i) t[i].join();

		return B;
	}
	
}


//===============================================================================Class_QRdecomposition=========================================================================================

namespace leo
{
	template<typename T>
	class QRdecomposition
	{
		private:
			int Size = 0;
			matrix<T> Q;
			matrix<T> R;
			matrix<T> A;
		public:
			QRdecomposition(){};
			
			void decQR(matrix<T> B);

			QRdecomposition(matrix<T> B) { decQR(B); };

			matrix<T> getQ() { return Q; };
			matrix<T> getR() { return R; };
			matrix<T> getA() { return A; };
			
			int size() { return Size * Size; };
			int square_size() { return Size; };
		private:
			void Gram_Schmidt_procces(matrix<T>& B);
			void Householder_transformation(matrix<T>& B);
	};

	template<typename T>
	void QRdecomposition<T>::Gram_Schmidt_procces(matrix<T>& B)
	{
		int N = B.size_row();
		int Ncol = B.size_col();

		this -> Q.resize(N,Ncol);
		this -> R.resize(N,N);
		this -> A = B;


		for (int k = 0; k < Ncol; ++k)
		{
			vector<T> a(N); std::copy(B.Column(k).begin(), B.Column(k).end(), a.begin());

			for (int i = 0; i < k; ++i)
			{
				this -> R[i][k] = scalarmult(a, this -> Q.Column(i));
				
				vector<T> q(N); std::copy(this -> Q.Column(i).begin(), this -> Q.Column(i).end(), q.begin());

				a -= this -> R[i][k] * q;
			}

			this -> R[k][k] = a.abs();
			if (this -> R[k][k] == 0) a /= this -> R[k][k] + 1e-15; //Noise!
			else a /= this -> R[k][k];

			std::copy(a.begin(), a.end(), this -> Q.Column(k).begin());
		}
	}


	template<typename T>
	void QRdecomposition<T>::Householder_transformation(matrix<T>& B)
	{
		if (!B.is_square()) throw std::invalid_argument("QR decomposition: matrix is not square!");
		
		int N = B.size_row();
		this -> Q = matrix<T>::identity(N);
		this -> R = B;
		this -> A = B;

		for (int k = 0; k < N - 1; ++k)
		{
			vector<T> X(N - k); std::copy(this -> R.Column(k).begin() + k, this -> R.Column(k).end(), X.begin());

			X[0] += (X[0] > 0 ? -1 : 1 ) * X.abs();
			T beta; //= 2 / scalarmult(X, X);
			if (scalarmult(X, X) < 1e-10) beta = 2 / (scalarmult(X, X) + 1e-15);  //Noise!
			else beta = 2 / scalarmult(X, X);
			

			for (int j = k; j < N; ++j)
			{
				T dot = 0;
				for (int i = k; i < N; ++i) dot += X[i - k] * this -> R[i][j];
				dot *= beta;

				std::transform(this -> R.Column(j).begin() + k, 
						this -> R.Column(j).end(), 
						X.begin(), 
						this -> R.Column(j).begin() + k,
						[dot] (T r, T x) { return r - dot * x;  });
			}

			for (int j = 0; j < N; ++j)
			{
				T dot = 0;
				for (int i = k; i < N; ++i) dot += X[i - k] * this -> Q[i][j];
				dot *= beta;

				std::transform(this -> Q.Column(j).begin() + k, 
						this -> Q.Column(j).end(),
						X.begin(),
						this -> Q.Column(j).begin() +k,
						[dot] (T q, T x) { return q - dot * x;  });
			}
		}
		
		//this -> R[N-1][N-1] *= -1;

		for (int i = 0; i < N; ++i ) for (int j = i + 1; j < N; ++j)
		{
			this -> R[j][i] = 0;
		}
		
	}


	template<typename T>
	void QRdecomposition<T>::decQR(matrix<T> B) 
	{
		if (!B.is_square()) this -> Gram_Schmidt_procces(B);
		else this -> Householder_transformation(B);
	}



}

//===============================================================================Linals_vectors=========================================================================================

namespace leo
{
	template<typename T>
	vector<T> Eigenvalues(matrix<T> A, bool shift = true, double Error = 1e-5, int N_max = 100000)
	{
		int N = A.size_row();
		QRdecomposition<T> qr(A);

		vector<T> It1(N);
		vector<T> It2(N);
		matrix<T> E = matrix<T>::identity(N);

		for (int i = 0; i < N; ++i) It1[i] = A[i][i];
		
		vector<T> lambda; lambda.reserve(N);

		while (N_max > 0)
		{
			N_max--;

			T mu = 0;
			if (shift) mu = A[N - 1][N - 1];

			qr.decQR(A -  mu * E);
			A = qr.getR()(qr.getQ()) + mu * E;
	
			 for (int i = 0; i < N; ++i) It2[i] = A[i][i];

			T error = (It2 - It1).abs();
			if (error < Error) break;

			It1 = It2;
		}

		for (int i = 0; i < A.size_row(); ++i) lambda.emplace_back(A[i][i]);
		
		return lambda;
	}

	template<typename T>
	matrix<T> Eigenvectors(matrix<T> A, bool shift = true, bool lmShift = false, int N_max = 100000, double Error = 1e-5)
	{
		std::vector<T> lambda = Eigenvalues(A, shift);

		int N = lambda.size();
		int n = A.size_row();

		matrix<T> Vecs(n, N);
		matrix<T> E = matrix<T>::identity(n);
		
		T MaxVal = *std::max_element(A.AllColumn().begin(), A.AllColumn().end());
		T MinVal = *std::min_element(A.AllColumn().begin(), A.AllColumn().end());

		std::vector<std::thread> t;
		for (int i = 0; i < N; ++i) t.push_back( std::thread( [&Vecs, &lambda, &A, &E, &Error, &lmShift, &n, &MaxVal, &MinVal, N_max, i]
		{		
			vector<T> X_past = vector<T>::randomvector(n, MaxVal, MinVal);
			vector<T> X_new = X_past; 

			LUdecomposition<T> lu(A - lambda[i] * E);
			
			int it = N_max;
			T lm = lambda[i];
		
			while (it > 0)
			{
				it--;

				if (lmShift) lu.decLU(A - lm * E);

				vector<T> Y = lu.solve(X_new);

				X_new = Y / Y.abs();

				T error = (X_new - X_past).abs();

				if (lmShift) lm = scalarmult(X_new, A(X_new)) / (scalarmult(X_new, X_new) + 1e-15); //Noise!
				/*{
					T scm = scalarmult(X_new, X_new);
				
					if (scm < 1e-8)  lm = scalarmult(X_new, A(X_new)) / (scm + 1e-15); //Noise!
					else lm = scalarmult(X_new, A(X_new)) / scm;
				}*/

				if (error < Error) break;

				X_past = X_new;
			}

			std::copy(X_new.begin(), X_new.end(), Vecs.Column(i).begin());
		}));

		for (int i = 0; i < N; ++i) t[i].join();


		std::map<int, std::vector<int>> mp;

		std::vector<std::pair<int, T>> sortlambda;
		for (int i = 0; i < lambda.size(); ++i) sortlambda.push_back({i, lambda[i]});
		std::sort(sortlambda.begin(), sortlambda.end(),
				[] (std::pair<int, T> a, std::pair<int, T> b)
				{ return a.second > b.second; } );

		int count = 0;
		for (int i = 0; i < N-1; ++i) 
		{
			T er = std::abs(sortlambda[i].second - sortlambda[i+1].second) / std::max({1.0, std::abs(sortlambda[i].second), std::abs(sortlambda[i+1].second)});

			if (er <= 1e-12)
			{
				if (!mp.count(count)) 
				{
					mp[count].push_back(sortlambda[i + 1].first); 
					mp[count].push_back(sortlambda[i].first);
				}
				else mp[count].push_back(sortlambda[i + 1].first);
			}
			else count++;
		}

		t.clear();
		for (auto it : mp) if (it.second.size() > 1) t.push_back( std::thread( [&Vecs, it, n]
		{
			int N_i = it.second.size();
			matrix<T> bais(n, N_i);
			
			for (int i = 0; i < N_i; ++i) std::copy(Vecs.Column(it.second[i]).begin(), 
								Vecs.Column(it.second[i]).end(), 
								bais.Column(i).begin());
		

			QRdecomposition<T> qr(bais);
			matrix<T> Q = qr.getQ();

			for (int i = 0; i < N_i; ++i) std::copy(Q.Column(i).begin(), 
								Q.Column(i).end(), 
								Vecs.Column(it.second[i]).begin());
		}));

		
		for (int i = 0; i < t.size(); ++i) t[i].join();

		return Vecs;
	}


	template<typename T>
	matrix<T> Eigenvectors(matrix<T> A, int etr, bool shift = true, bool lmShift = false, int N_max = 100000, double Error = 1e-2)
	{
		int N = A.size_row();
		matrix<T> Vec = Eigenvectors(A, shift, lmShift, N_max, Error);
		matrix<T> Vec_first = Vec;

		int etr_it = etr;
		
		while (etr_it > 1)
		{
			etr_it--;
			
			matrix<T> Mit = Eigenvectors(A, shift, lmShift, N_max, Error);

			for (int i = 0; i < N; ++i)
			{
				T idx = scalarmult(Vec_first.Column(i), Mit.Column(i));
				if (idx < 0) for (auto& it : Mit.Column(i)) it = -it;
			}

			Vec += Mit;
	
		}
	
		Vec /= static_cast<double>(etr);

		for (int i = 0; i < N; ++i)
		{
			T norm = scalarmult(Vec.Column(i), Vec.Column(i));
			for (auto& it : Vec.Column(i)) it /= norm;
		}
		
		return Vec;
	}
	


}


//===============================================================================SVD_decomposition=========================================================================================

namespace leo
{
	template<typename T>
	class SVDdecomposition
	{
		private:
			int m = 0;
			int n = 0;
			matrix<T> A;
			matrix<T> U;
			matrix<T> S;
			matrix<T> V;
		public:
			SVDdecomposition(){};

			void decSVD(matrix<T> B);

			SVDdecomposition(matrix<T> B){ decSVD(B); };

			matrix<T> getA(){ return A; }
			matrix<T> getU(){ return U; }
			matrix<T> getS(){ return S; }
			matrix<T> getV(){ return V; }

			SVDdecomposition<T> compress(int k);
	};


	template<typename T>
	void SVDdecomposition<T>::decSVD(matrix<T> B)
	{
		this -> m = B.size_row();
		this -> n = B.size_col();
		int rank = std::min(m,n);
		std::vector<std::thread> t;

		this -> A = B;

		matrix<T> AtA = B.transposition()(B);

		struct sig { int first; T second; };

		std::vector<sig> sortlambda;

		t.push_back( std::thread([this, &AtA] {this -> V = Eigenvectors(AtA, true, true);}) );
		t.push_back( std::thread([this, &AtA, &rank, &sortlambda] 
		{
			this -> S = matrix<T>(m,n);

			vector<T> sigma = Eigenvalues(AtA);
			for (int i = 0; i < sigma.size(); ++i) sortlambda.push_back({i, sigma[i] });			
			for (int i = 0; i < rank; ++i) this -> S[i][i] = std::sqrt((sigma[i] >= 0 ? sigma[i] : 0));
		}));
	
		for (int i = 0; i < t.size(); ++i) t[i].join();

		std::sort(sortlambda.begin(), sortlambda.end(),
			[] (sig a, sig b)
			{ return a.second > b.second; } );

		matrix<T> V_new = this -> V;
		matrix<T> S_new = this -> S;

		for (int i = 0; i < rank; ++i)
		{
			int num = sortlambda[i].first;
			std::copy(V_new.Column(num).begin(), V_new.Column(num).end(), this -> V.Column(i).begin());
			std::swap(this -> S[i][i], S_new[num][num]);
		}

		this -> U = A(V);

		for (int i = 0; i < m; ++i) for (int j = 0; j < m; ++j)
		{
			this -> U[j][i] /= (this->S[i][i] > 0 ? this->S[i][i] : 1e-15); //Noise!
		}
		
	}


	//template<typename T>
	//void SVDdecomposition<T>::decSVD(matrix<T> B){}



	template<typename T>
	SVDdecomposition<T> SVDdecomposition<T>::compress(int k)
	{
		if (!n || !m) throw std::invalid_argument("SVD decomposition ::compress: decomposition is empty! (No init)");
		if (k > n || k > m) throw std::invalid_argument("SVD decomposition ::compress: size of compression more then rank of matrix!");

		SVDdecomposition<T> svd = *this;
		this -> U.resize(m, k);
		this -> S.resize(k, k);
		this -> V.resize(n, k);
		this -> A = U(S(V.transposition()));

		SVDdecomposition<T> svd_copy = *this;
		this -> A = svd.getA();
		this -> U = svd.getU();
		this -> S = svd.getS();
		this -> V = svd.getV();

		return svd_copy;
	}


	template<typename T>
	matrix<T> PCA(matrix<T> A, int k)
	{
		int N = A.size_col();
		int n = A.size_row();
		
		for (int i = 0; i < N; ++i)
		{
			T sum = 0.0;
			for (auto& it : A.Column(i)) sum += it;

			sum /= n;
			for (auto& it : A.Column(i)) it -= sum;
		}

		SVDdecomposition<T> svd(A);
		matrix<T> V = svd.getV(); V.resize(N, k);

		A = A(V);

		return A;
	}

}



//===============================================================================Solve_linal_methods=========================================================================================

namespace leo{

template<class T>
class Regression
{
    private:
        bool _fit_init_ = false;
        int level = 1;
        int space = 1;
        std::vector<T> cofs = {0, 0};
        
        void change_level(size_t l);
    
        void change_space(size_t s);
    public:
        Regression(size_t l, size_t s) : level(l), space(s)
        {
            cofs.resize(l * s + 1);
        }

        Regression(size_t l) : level(l)
        {
             cofs.resize(l + 1);
        }

        template<typename U1, typename U2>
        std::vector<T> fit(std::vector<U1> X, std::vector<U2> Y);

        template<typename U1, typename U2>
        std::vector<T> fit(matrix<U1> X, std::vector<U2> Y);

        template<typename Iterator1, typename Iterator2>
        std::vector<T> fit(Iterator1 X_begin, Iterator1 X_end, Iterator2 Y_begin, Iterator2 Y_end);

        template<typename U1>
        std::vector<T> predict(std::vector<U1> X);

        template<typename U1>
        std::vector<T> predict(matrix<U1> X);

        template<typename Iterator1>
        std::vector<T> predict(Iterator1 X_begin, Iterator1 X_end);

        template<typename U1>
        void ignore(U1 func);

        template<typename U1>
        void quest(U1 func);
};

template<class T>
void Regression<T>::change_level(size_t l)
{
    this -> level = l;
    cofs.resize(l * this -> space + 1);
}

template<class T>
void Regression<T>::change_space(size_t s)
{
    this -> space = s;
    cofs.resize(this -> level * s + 1);
}


template<class T>
template<typename Iterator1, typename Iterator2>
std::vector<T> Regression<T>::fit(Iterator1 X_begin, Iterator1 X_end, Iterator2 Y_begin, Iterator2 Y_end)
{
    auto X_len = std::distance(X_begin, X_end);
    auto Y_len = std::distance(Y_begin, Y_end);
    
    if (X_len != Y_len) throw std::invalid_argument("Error in Regression::fit : size of X and Y not equel!");

    if (this -> space != 1) this -> change_space(1);

    int lenght = this -> cofs.size();

    matrix<T> AX_vec(X_len, lenght);
    std::vector<T> AY_vec(X_len);

    for (int i = 0; i < X_len; ++i)
    {
        for (int j = 0; j < lenght - 1; ++j)
        {
            T value = 1;
            for (int k = 0; k < this -> level - j; ++k)
            {
                value *= *(X_begin + i);
            }

            AX_vec[i][j] = value;
        }

        AX_vec[i][lenght - 1] = 1;

        AY_vec[i] = *(Y_begin + i);
    }

    auto cof  = solve(AX_vec, AY_vec);
    
    this -> cofs = cof;
    this -> _fit_init_ = true;

    return cof;
}


template<class T>
template<typename U1, typename U2>
std::vector<T> Regression<T>::fit(std::vector<U1> X, std::vector<U2> Y)
{
    return this -> fit(X.begin(), X.end(), Y.begin(), Y.end());
}


template<class T>
template<typename U1, typename U2>
std::vector<T> Regression<T>::fit(matrix<U1> X, std::vector<U2> Y)
{
    int lenght_Y = Y.size();

    if (X.size_row() != lenght_Y) throw std::invalid_argument("Error in Regression::fit : size of X and Y not equel!");

    if (this -> space != X.size_col()) this -> change_space( X.size_col() + 1 );

    int num_features = X.size_col();
    int degree = this -> level;

    int lenght = num_features * degree + 1;

    matrix<U1> X_new(X.size_row(), lenght);

    for (size_t n = 0; n < X.size_row(); ++n)
    {
        size_t col = 0;
        
        for (size_t f = 0; f < num_features; ++f)
        {
            U1 value = X[n][f];
            U2 power = value;

            for (size_t d = 1; d <= degree; ++d)
            {
                X_new[n][col++] = power;
                power *= value;
            }
        }
    
        X_new[n][lenght - 1] = static_cast<U1>(1);
    }

    auto cof  = solve(X_new, Y);

    this -> cofs = cof;
    this -> _fit_init_ = true;

    return cof;
}


template<class T>
template<typename Iterator1>
std::vector<T> Regression<T>::predict(Iterator1 X_begin, Iterator1 X_end)
{
    if (!this -> _fit_init_) throw std::invalid_argument("Error in Regression class predict function: fit function did't work first!");

    if (this -> space != 1) throw std::invalid_argument("Error in Regression class predict function: unexcepted matrix in space = 1!");

    std::vector<T> result;
    std::copy(X_begin, X_end, std::back_inserter(result));

    for (auto& it : result)
    {
        T value = this -> cofs[level];

        for (int i = 0; i + 1 < this -> cofs.size(); ++i)
        {
            T value_dop = 1;
            for (int k = 0; k < level - i; ++k)
            {
                value_dop *= it;
            }

            value += this -> cofs[i] * value_dop;
        }

        it = value;
    }

    return result;
}


template<class T>
template<typename U1>
std::vector<T> Regression<T>::predict(std::vector<U1> X)
{
    return predict(X.begin(), X.end());
}


template<class T>
template<typename U1>
std::vector<T> Regression<T>::predict(matrix<U1> X)
{
    if (!this -> _fit_init_) throw std::invalid_argument("Error in Regression class predict function: fit function did't work first!");

    if (this -> space != X.size_col()) throw std::invalid_argument("Error in Regression class predict function: missspacing of matrix!");
    
    int degree = this -> level;
    int num_features = this -> space;
    int lenght = X.size_row();

    std::vector<T> result;
    result.reserve(lenght);
    

    for (int i = 0; i < lenght; ++i)
    {
        T value = 0.0;

        for (int s = 0; s < num_features; ++s)
        {
            for (int d = 1; d <= degree; ++d)
            {
                T power = 1;
                for (int j = 0; j < d; ++j)
                {
                    power *= X[i][s];
                }
                power *= this -> cofs[s * degree + d - 1];
                value += power;
            }
        }

        value += this -> cofs[num_features * degree];
        result.push_back(value);
    }

    return result;

}



}




