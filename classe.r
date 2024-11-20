library(ggplot2)
library(tidyverse)

X <- c(
  10.1, 10.8, 10.3, 10, 8.4, 8.7, 7.6, 8.3, 10.7, 9.4,
  8.5, 9.3, 9.7, 10.5, 11.4, 10.2, 10.4, 8.2, 9.1, 9.6
)

summary(X)

ggplot() +
  geom_histogram(aes(x = X, y = after_stat(density)),
    bins = 8,
    col = "black", fill = "#E0E0E0"
  ) +
  geom_function(aes(col = "Normal"),
    linewidth = 1,
    fun = function(x) dnorm(x, mean(X), sd(X))
  ) +
  geom_function(aes(col = "Uniforme"),
    linewidth = 1,
    fun = function(x) dunif(x, min(X), max(X))
  ) +
  labs(
    title = "Histograma d'X i densitats teòriques",
    y = "Densitat", col = "Models"
  )

dX <- tibble(x = sort(c(X, 7, 12)))
dF <- mutate(dX,
  Fx = sapply(x, function(x) mean(X <= x))
)
ggplot() +
  geom_step(data = dF, aes(x = x, y = Fx)) +
  geom_function(aes(col = "Normal"),
    linewidth = 1,
    fun = function(x) pnorm(x, mean(X), sd(X))
  ) +
  geom_function(aes(col = "Uniforme"),
    linewidth = 1,
    fun = function(x) punif(x, min(X), max(X))
  ) +
  labs(col = "Model", y = "Funció de distribució")

dX <- tibble(x = sort(X))
dF <- mutate(dX,
  Fx = sapply(x, function(x) mean(X <= x))
)


dF <- dF %>%
  mutate(
    Fx_norm = pnorm(x, mean(X), sd(X)),
    Fx_unif = punif(x, min(X), max(X))
  )
ggplot(data = dF) +
  geom_point(aes(x = Fx_norm, y = Fx, col = "Normal")) +
  geom_point(aes(x = Fx_unif, y = Fx, col = "Uniforme")) +
  geom_abline(intercept = 0, slope = 1, linetype = "dotted") +
  labs(
    x = "Funció de distribució teòrica",
    y = "Funció de distribució de la mostra",
    col = "Model"
  )

dX <- tibble(x = sort(X))
dQ <- mutate(dX,
  prob = ppoints(n())
)

dQ <- mutate(dQ,
  qNorm = qexp((prob, mean = 1 / mean(X), sd = sd(X)),
  qUnif = qunif(prob, min = min(X), max = max(X))
)
ggplot() +
  geom_point(data = dQ, aes(x = qNorm, y = x, col = "Normal")) +
  geom_point(data = dQ, aes(x = qUnif, y = x, col = "Uniforme")) +
  geom_abline(intercept = 0, slope = 1, linetype = "dotted") +
  labs(
    col = "Models",
    x = "Quantil teòric",
    y = "Quantil mostral"
  )

X2 <- c(
  15.41, 27.46, 208.05, 7.76, 28.99, 68.56, 160.23, 19.37, 162.6, 2.37,
  164.33, 31.59, 75.28, 20.81, 2.48, 110.82, 33.23, 23.76, 8.58, 6.4,
  595.77, 5.9, 33.44, 47.47, 36.42, 21.44, 32.57, 158.37, 125.96, 29.71
)

shapiro.test(X2)
