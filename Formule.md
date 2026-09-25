# GIS Formule

## $V_{shale}$

1. $ V_{shale} (z) = \dfrac{U_{SP} (Z)  - U_{SP, min}}{U_{SP, max}  - U_{SP, min}} $

2. $ V_{shale} (z) \approx I_{GR} = \dfrac{GR (Z)  - GR_{min}}{GR_{max}  - GR_{min}} $

## $K_{П}$

1. $ K_{П,GNT} (Z) = K_{П, \text{solid}} + ( K_{П, shale} - K_{П, \text{solid}} ) I_{GNT} (Z) $ :

$$ I_{GNT} (Z) = \dfrac{ GNT (Z) - GNT_{\text{solid}} }{ GNT_{shale}  - GNT_{ \text{solid} }} $$

2. $  K_{П,SL} (z) = C \dfrac{ \Delta t_{SL} (Z) - \Delta t_{SL,sc}}{ \Delta  t_{SL} (Z) } $ :

$$ C \approx \overline{0.624, 0.7} $$

3. $ K_{П,SL} (z) = \dfrac{ \Delta t_{SL} (Z) - \Delta t_{SL,sc}}{ \Delta t_{SL,f} - \Delta t_{SL,sc}} $

$$
\Delta t_{SL} = (1 - K_{П} - V_{shale}) \Delta t_{sc} + K_{shale} \Delta t_{shale} + K_{П} \Delta t_{f} \Rightarrow
$$

4. $ K_{П,SL} (z) = \dfrac{ \Delta t_{SL} (Z) - \Delta t_{SL,sc}}{ \Delta t_{SL,f} - \Delta t_{SL,sc}} + V_{shale} \dfrac{\Delta t_{SL,sc} - \Delta t_{SL,shale}}{\Delta t_{SL,f} - \Delta t_{SL,sc}} $

## $K_{П.ЭФ}$

1. $K_{П.ЭФ} (Z) = K_{П,GNT} (Z) - K_{П,shale} V_{shale} (Z) $

## $ S_{Wi}$

1. $S_{W} = \sqrt[n]{ \dfrac{a}{\phi^m} \dfrac{\rho_f}{\rho (Z)} }$

2. $S_{W} = \sqrt[n]{ \dfrac{a}{\phi^m} (\dfrac{1}{\rho (Z)} - \dfrac{V_{shale}}{\rho_{shale}}) (\dfrac{\rho_f}{1 - V_{shale}})} $ - layers clay

3. $S_W = \dfrac{a \rho_f}{2 \phi^m} (- \dfrac{V_{shale}}{\rho_{shale}} + \sqrt{ (\dfrac{V_{shale}}{\rho_{shale}})^2 + \dfrac{4 \phi^m}{a \rho (Z) \rho_{f}} })$

4. $S_{W} = \dfrac{1}{ \sqrt[\dfrac{n}{2}]{ (\dfrac{\phi^{\dfrac{m}{2}}}{\sqrt{a \rho (Z)}} + \dfrac{V_{shale}^{1 - \dfrac{V_{shale}}{2}}}{\sqrt{\rho_{shale}}})\sqrt{\rho (Z)} } }$

5. $S_{W} = \dfrac{-B Q_V + \sqrt{B^2 Q_V + \dfrac{4 F}{\rho_f \rho (Z)}}}{2}$ :

$$
B = 4.6 -2.76 e^{- \dfrac{0.77}{\rho_f}}
$$

$$
Q = \dfrac{2.8 \\ 0.2 V_{shale}}{\phi_{eff}}
$$

$$
F = \phi_{eff} - 1.8
$$

## $K_{ПР}$

1. $K_{ПР} (K_{П} (Z)) = e^{a K_{П} (Z) + b}$

2. $K_{ПР} (\text{FZI}) = \phi ( \dfrac{\phi \text{FZI}}{0.0314 (1 - \phi)})^2$

3. $K_{ПР} = ( \dfrac{250 K_П^3}{S_{Wi}} )^2$

4. $K_{ПР} = ( \dfrac{100 K_П^{2.25}}{S_{Wi}} )^2$

5. $K_{ПР} = ( \dfrac{300}{W^4} \dfrac{K_П^W}{S_{Wi}})^2$

6. $K_{ПР} =  (100 \dfrac{(1 - S_{Wi})K_П^2}{S_{Wi}})^2$

## $\lambda_{PNL}$

$ \lambda_{PNL} = \dfrac{\ln{I_1 (z, t)} - \ln{I_2 (z, t)}}{T_2 (Z) - T_1 (Z)} $


## $\alpha_{SL}$

$ \alpha_{SL} = \dfrac{1}{\Delta L} \ln{\dfrac{A_1}{A_2}}$

# Imperical formule

## $\rho$ and $K_П$

### 1. Archi formula

$$
S = S_f \phi^m p^n \Rightarrow \phi = (\dfrac{S (Z)}{S_f})^{\dfrac{1}{m}}
$$

$m \in (1, 2)$

$n \approx 2$

$C \approx 10$

$$
\Rightarrow \phi = C \dfrac{S (Z)}{S_f}
$$

### 2. Baussons model

$$
S (Z) = \dfrac{1 - \dfrac{S_{shale}}{S_f}}{1 - \dfrac{S_{shale}}{S (Z)}} S_f \phi^m
$$

### 3. Harnans model

$$
S (Z) = (S_f - S_{shale}) \phi^m + S_{shale} = (1 - \phi^m) S_{shale} - S_f \phi^m
$$

$$
\Rightarrow \phi (Z) = ( \dfrac{S (Z) - S_{shale}}{S_f - S_{shale}} )^{\dfrac{1}{m}}
$$

$ \lim_{S_{shale} \to 0}{ (\dfrac{S (Z) - S_{shale}}{S_f - S_{shale}})^{\dfrac{1}{m}} } = (\dfrac{S (Z)}{S_f})^{\dfrac{1}{m}} $ - Archi formula

### 4. Glovers model

$$
S (Z) = (1 - \phi)^p S_{shale} + S_f \phi^m
$$

$ p = \log_{1 - \phi}{1 - \phi^m} \Rightarrow S(Z) = (1 - \phi^m) S_{shale} - S_f \phi^m $ - Harnans model

## Bifas

### Boarders

$$
S_{max} = S_m + \dfrac{C_S}{\dfrac{1}{S_s - S_m} + \dfrac{C_m}{3 S_m}}
$$

$$
S_{min} = S_S + \dfrac{C_m}{\dfrac{1}{S_m - S_s} + \dfrac{C_s}{3 S_s}}
$$

### Model of sfers 

$$
S (Z) = \dfrac{S_m + (S_s - S_m) (1 - \dfrac{2}{3} C_m)}{1 + \dfrac{1}{3} C_m \dfrac{S_s}{S_m - 1}} \Rightarrow C_m = 3 S_m \dfrac{S (Z) - S_s}{(S (Z) + 2 S_m)(S_m - S_s)}
$$

$S_m >> S_f \Rightarrow S (Z) \approx \dfrac{2}{3} C_m \dfrac{S_m}{1 - \dfrac{1}{3}C_M}$

$C_m << 1 \Rightarrow S (Z) = \dfrac{2}{3} C_m S_m$

## Bifas whit cub

$$
S (Z) = \dfrac{S_s S_m (1 - C_m)^{\dfrac{2}{3}}}{S_m (1 - C_m)^{\dfrac{1}{3}} + S_s (1 - (1 - C_m)^{\dfrac{1}{3}})} + S_m (1 - (1 - C_m)^{\dfrac{2}{3}})
$$

$S_m >> S_s \Rightarrow S (Z) \approx S_m S_m (1 - (1 - C_m)^{\dfrac{2}{3}})$

$C_m << 1 \Rightarrow S (Z) \approx \dfrac{2}{3} C_m S_m$

### Hashin - Shtrihman models

$$
S_{min} = S_s \leq S_f = S_{max} :
$$

$$
S_{HS}^- = \dfrac{1}{ \dfrac{1- \phi}{3 S_s} + \dfrac{\phi}{S_f + 2 S_s} } - 2 S_s
$$

$$
S_{HS}^+ = \dfrac{1}{ \dfrac{1- \phi}{S_f + 2 S_s} + \dfrac{\phi}{3 S_f} } - 2 S_f
$$

$ S_{min} = S_f \leq S_s = S_{max} $ :

$$
\dfrac{3 - \phi}{2 \phi} = \dfrac{S_f}{S_{HS}^+}
$$ 

$$
\Rightarrow \phi = (\dfrac{S_s - S (Z)}{S_s - S_f})(\dfrac{3 S_f}{S (Z) + S_f}) \text{ - up boarde}
$$

$$
\Rightarrow \phi = (\dfrac{S_s - S (Z)}{S_s - S_f})(\dfrac{S_f + 2 S_s}{S (Z) + 2 S_s}) \text{ - bottem boarde}
$$

#### Auto balance model

$$
\phi = (\dfrac{S (Z) - S_s}{S_f - S_s})(\dfrac{S_f}{S (Z)})^{1 - \dfrac{1}{m}}
$$

if sfer $\Rightarrow 1 - \dfrac{1}{m} = \dfrac{1}{3}$
