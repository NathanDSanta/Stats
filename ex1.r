library(unigd)
ugd(width = 300, height = 300)

t <- seq(0, 10, 0.1)
y <- sin(t)

plot(t, y, type = "l")
