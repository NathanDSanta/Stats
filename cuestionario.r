library("nycflights13")
library("tidyverse")
data(package = "nycflights13")

glimpse(flights)

aeroports_dtw_den <- filter(flights, dest == "DTW" | dest == "DEN")

nrow(aeroports_dtw_den)

percent <- nrow(filter(aeroports_dtw_den, origin == "LGA")) / nrow(aeroports_dtw_den)

taula_kmh <- mutate(flights,
  kmh = distance * 1.60934 / (air_time / 60)
)

taula_kmh_dtw <- filter(taula_kmh, origin == "LGA" & dest == "DTW")

taula_kmh_dtw <- filter(taula_kmh_dtw, !is.na(kmh))

sum(taula_kmh_dtw[["kmh"]]) / nrow(taula_kmh_dtw)

taula <- filter(flights, origin == "JFK" & (dest == "PSE" | dest == "JAC" | dest == "SDF" | dest == "SRQ"))

taula <- filter(flights, !is.na(dep_delay) & !is.na(arr_delay))

summarise(taula,
  correlacio = cor(dep_delay, arr_delay),
  cov(dep_delay, arr_delay) / (sd(dep_delay) * sd(arr_delay))
)

taula <- filter(flights, origin == "EWR" | dest == "MSP")

hist(flights,
  x = month,
  y = count(n)
)
