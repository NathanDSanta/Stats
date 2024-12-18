# Cargar la librería ggplot2 para la visualización
library(ggplot2)

# Cargar el archivo RData con los resultados
load("tiemposMedidas.RData")

# Filtrar los resultados para solo incluir MergeSort y QuickSort
resultados_filtrados <- subset(resultados, Algorithm %in% c("MergeSort", "QuickSort"))

# Crear la gráfica comparativa de tiempos para MergeSort y QuickSort, con facetado por CaseType
ggplot(resultados_filtrados, aes(x = Size, y = TimeSeconds, color = Algorithm)) +
  geom_point() +
  geom_smooth(method = "lm", se = FALSE) +
  scale_color_manual(values = c("MergeSort" = "blue", "QuickSort" = "red"))+
  facet_wrap(~ CaseType, scales = "free_y") +  # Crear una gráfica por cada tipo de caso
  labs(
    title = "Comparación de Tiempos: MergeSort vs QuickSort",
    x = "Tamaño del Array",
    y = "Tiempo (segundos)"
  ) +
  theme_minimal() +
  theme(legend.title = element_blank())

load("grafiques.RData")
# Filtrar los resultados para solo incluir MergeSort y QuickSort
resultados_filtrados <- subset(data, Algoritme %in% c("MergeSort", "QuickSort"))

resultados_filtrados <- subset(resultados_filtrados, Temps <= 130)

# Gráfica combinada con leyenda personalizada
ggplot() +
  # Puntos de QuickSort en rojo
  geom_point(aes(x = Coeficient.Ordenacio, y = Temps, color = "Puntos QuickSort"), 
             data = subset(resultados_filtrados, Algoritme == "QuickSort")) +
  # Puntos de MergeSort en azul
  geom_point(aes(x = Coeficient.Ordenacio, y = Temps, color = "Puntos MergeSort"), 
             data = subset(resultados_filtrados, Algoritme == "MergeSort")) +
  # Curva cuadrática para QuickSort en púrpura
  geom_smooth(aes(x = Coeficient.Ordenacio, y = Temps, color = "Curva QuickSort"), 
              data = subset(resultados_filtrados, Algoritme == "QuickSort"),
              method = "lm", formula = y ~ poly(x, 2), se = FALSE) +
  # Curva cuadrática para MergeSort en verde
  geom_smooth(aes(x = Coeficient.Ordenacio, y = Temps, color = "Curva MergeSort"), 
              data = subset(resultados_filtrados, Algoritme == "MergeSort"),
              method = "lm", formula = y ~ poly(x, 2), se = FALSE) +
  # Personalización de colores en la leyenda
  scale_color_manual(values = c("Puntos QuickSort" = "red", 
                                "Puntos MergeSort" = "blue", 
                                "Curva QuickSort" = "purple", 
                                "Curva MergeSort" = "green")) +
  # Títulos y etiquetas
  labs(
    title = "Puntos y curvas cuadráticas ajustadas para MergeSort y QuickSort",
    x = "Coeficient d'Ordenació",
    y = "Tiempo (microsegundos)",
    color = "Elementos"  # Título de la leyenda
  ) +
  theme_minimal()

# Per més claredat, veiem les corbes sense els punts:

# Gráfica solo con las curvas cuadráticas para MergeSort y QuickSort
ggplot() +
  # Curva cuadrática para QuickSort en púrpura
  geom_smooth(aes(x = Coeficient.Ordenacio, y = Temps, color = "Curva QuickSort"), 
              data = subset(resultados_filtrados, Algoritme == "QuickSort"),
              method = "lm", formula = y ~ poly(x, 2), se = FALSE) +
  # Curva cuadrática para MergeSort en verde
  geom_smooth(aes(x = Coeficient.Ordenacio, y = Temps, color = "Curva MergeSort"), 
              data = subset(resultados_filtrados, Algoritme == "MergeSort"),
              method = "lm", formula = y ~ poly(x, 2), se = FALSE) +
  # Personalización de colores en la leyenda
  scale_color_manual(values = c("Curva QuickSort" = "purple", 
                                "Curva MergeSort" = "green")) +
  # Títulos y etiquetas
  labs(
    title = "Curvas cuadráticas ajustadas para MergeSort y QuickSort",
    x = "Coeficient d'Ordenació",
    y = "Tiempo (microsegundos)",
    color = "Algoritmo"  # Título de la leyenda
  ) +
  theme_minimal()
