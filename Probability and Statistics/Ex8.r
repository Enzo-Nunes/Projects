library(pracma)
set.seed(1592)

data <- c(31.8, 31.7, 35.2, 37.1, 31.7, 36.1, 36.3, 33.2, 34.3, 37.5, 30.4, 34.6, 32.4, 31.7, 30.2, 34.3, 35.6, 34.9,
		  38.9)

# Calculate the sample variance
sample <- sample(data, 12, replace = FALSE)
n <- length(sample)
sample_variance <- var(sample)

# Calculate the first Confidence Interval for sigma^2
gamma <- 0.96
a <- qchisq((1 - gamma) / 2, df = n - 1)
b <- qchisq((1 + gamma) / 2, df = n - 1)
ci_lower <- (n - 1) * sample_variance / b
ci_upper <- (n - 1) * sample_variance / a
ci_amplitude <- ci_upper - ci_lower

f <- function(x) {
    c <- x[1]
    d <- x[2]
    eq1 <- pchisq(d, df = n - 1) - pchisq(c, df = n - 1) - gamma
    eq2 <- dchisq(d, df = n + 3) - dchisq(c, df = n + 3)
    return(c(eq1, eq2))
}

# Estimate the new values for c and d
initial_guess <- c(a, b)
solution <- fsolve(f, initial_guess)
c <- solution$x[1]
d <- solution$x[2]

# Calculate the new confidence interval for sigma^2
ci_lower_new <- (n - 1) * sample_variance / d
ci_upper_new <- (n - 1) * sample_variance / c
ci_amplitude_new <- ci_upper_new - ci_lower_new

amplitude_difference <- abs(ci_amplitude_new - ci_amplitude)
amplitude_difference_rounded <- round(amplitude_difference, 3)

print(amplitude_difference_rounded)