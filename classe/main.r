setwd("~/Documents/uni/Stats")
library(tidyverse)
library(readxl)
salaris <- read_excel("salary.xlsx")

y <- salaris$sl

SQT <- sum((y - mean(y))^2)
SQT

var(y)
SQT / (52 - 1)

m1 <- nls(sl ~ a * exp(b * yr), data = salaris, list(a = 15000, b = 0.2))
m1
coef(m1)

a <- coef(m1)[1]
b <- coef(m1)[2]
f_ajustada <- function(x) a * exp(b * x)
ggplot(data = salaris) +
  geom_point(aes(x = yr, y = sl)) +
  geom_function(fun = f_ajustada, col = "blue")

m.yr <- lm(sl ~ yr, data = salaris)
m.yr

sum(residuals(m.yr)^2)

sum(residuals(m1)^2)

a <- coef(m1)[1]
b <- coef(m1)[2]
f_ajustada <- function(x) a * exp(b * x)
ggplot(data = salaris) +
  geom_point(aes(x = yr, y = sl)) +
  geom_function(fun = f_ajustada, col = "blue") +
  geom_abline(intercept = 18166.1, slope = 752.8, col = "red")

SQR <- sum(residuals(m.yr)^2)
SQR

f.yr <- function(x) coef(m.yr)[1] + coef(m.yr)[2] * x
sum((y - f.yr(salaris$yr))^2)

1 - SQR / SQT

SQM <- sum((mean(y) - f.yr(salaris$yr))^2)
SQM

SQM + SQR
SQT

m.yr_yd <- lm(sl ~ yr + yd, data = salaris)
m.yr_yd

library(scatterplot3d)
s3d <- with(salaris, scatterplot3d(yr, yd, sl, angle = 60, box = FALSE))
s3d$plane3d(m.yr_yd, col = "red")

x_associate = as.integer(salaris$rk == 'associate')
x_full = as.integer(salaris$rk == 'full')
m.rk_manual = lm(sl~x_associate+x_full, salaris)
m.rk_manual

m.rk = lm(sl~rk, salaris)
m.rk

sl_mitja_model = tribble(~rk,~sl,
                         'assistant', 17769,
                         'associate', 17769+5407,
                         'full', 17769+11890)

ggplot(data=salaris) +
  geom_jitter(aes(x = '', y = sl)) +
  geom_hline(data = sl_mitja_model, aes(yintercept = sl)) +
  facet_wrap(~rk)

m.yr2 = lm(sl~yr+yr2, data = mutate(salaris, yr2 = yr^2))
m.yr2

SQR2 = sum(residuals(m.yr2)^2)
SQR2

(SQT-SQR2)/SQT

ggplot(data=salaris) +
  geom_point(aes(x = yr, y = sl)) +
  geom_function(fun = function(x) 17644.157+926.432*x-9.064*x^2, col = 'red')

m.rk_yr = lm(sl~rk+yr, data = salaris)
ggplot(data = salaris) +
  geom_point(aes(x = yr, y = sl, col = rk)) +
  geom_abline(aes(intercept = 16203.3, slope = 375.7, col = 'assistant')) +
  geom_abline(aes(intercept = 16203.3+4262.3, slope = 375.7, col = 'associate')) +
  geom_abline(aes(intercept = 16203.3+9454.5, slope = 375.7, col = 'full'))

m.rk_yr_inter = lm(sl~rk+yr+rk:yr, data = salaris)
m.rk_yr_inter

ggplot(data = salaris) +
  geom_point(aes(x = yr, y = sl, col = rk)) +
  geom_abline(aes(intercept = 16416.6, slope = 324.5, col = 'assistant')) +
  geom_abline(aes(intercept = 16416.6+5354.2, slope = 324.5-129.7, col = 'associate')) +
  geom_abline(aes(intercept = 16416.6+8176.4, slope = 324.5+151.2, col = 'full'))

 