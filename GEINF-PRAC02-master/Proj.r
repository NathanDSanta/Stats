# setwd("GEINF-PRAC02-master") # nolint: commented_code_linter.
library("tidyverse")
load("movies_shows.RData")
library("RColorBrewer")
display.brewer.all(colorblindFriendly = TRUE)

glimpse(movies)
dim(movies)

summarise(movies, n.cat = n_distinct(country))

recompte <- count(movies, country)
arrange(recompte, desc(n))

recompte_sense_NA <- filter(recompte, !is.na(country)) # nolint
arrange(recompte_sense_NA, desc(n))

x <- c("A" = 2, "B" = 4, "C" = 8)
barplot(x) # nolint
pie(x)

x <- pull(arrange(recompte, desc(n)), n, name = country)
barplot(x, las = 2, cex.names = 0.7) # nolint

country_20 <- slice_max(recompte_sense_NA, n, n = 20)

x <- pull(country_20, name = country)
barplot(x, las = 2, cex.names = 0.7) # nolint

summarise(movies,
  n.na = sum(is.na(added_year))
)

summarise(movies,
  mitjana = mean(added_year, na.rm = TRUE),
  mediana = median(added_year, na.rm = TRUE)
)

summarise(filter(movies, !is.na(added_year)),
  mínim = min(added_year),
  màxim = max(added_year),
  `1r quartil` = quantile(added_year, 0.25),
  `3r quartil` = quantile(added_year, 0.75),
  RIQ = IQR(added_year)
)

hist(movies$added_year,
  main = "Histograma",
  xlab = "Any d'incorporació a Netflix", ylab = "Freqüència"
)

boxplot(movies$added_year,
  main = "Gràfic de caixa",
  xlab = "Any d'incorporació a Netflix",
  horizontal = TRUE
)

x <- table(movies$added_year)
barplot(x, las = 2, cex.names = 0.7)

taula_freq_abs <- with(
  movies,
  table(united_states, movie_duration_cat)
)
taula_freq_abs

dades_figura <- select(movies, release_year, movie_duration)
plot(dades_figura, col = brewer.pal(n = 5, name = "PRGn"))

dades <- tibble(
  x = runif(100),
  y = 10 * x + rnorm(100)
)
plot(dades)
summarise(dades,
  correlació = cor(x, y),
  cov(x, y) / (sd(x) * sd(y))
)
