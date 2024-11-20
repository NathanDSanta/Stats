library(ggplot2)
library(tidyverse)

X <- c(1, 0, 1, 0, 1, 1, 1, 1)

summary(X)

ggplot() +
  geom_histogram(aes(x = X, y = after_stat(density)),
    bins = 2,
    col = "black", fill = "purple"
  ) +
  geom_function(aes(col = "Normal"),
    linewidth = 1,
    fun = function(x) dbinom(x, 8, 0.5)
  )
