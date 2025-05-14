library(stats4)

data <- c(8.54, 4.76, 5.15, 4.96, 6.25, 7.22, 12.9, 6.04, 8.86, 4.88, 6.54, 4.53, 4.7, 5.38, 5.96, 5.17, 5.09, 5.11)
a <- 4.5
true_theta <- 3.4

neg_log_likelihood <- function(theta) {
	if (theta <= 0) return(Inf)
	n <- length(data)
	sum_log <- sum(log(data))
	log_likelihood <- n * log(theta) + theta * n * log(a) - (theta + 1) * sum_log
		return(-log_likelihood)
}

mle_estimate <- mle(neg_log_likelihood, start = list(theta = 3.4))
theta_hat <- coef(mle_estimate)
quantile_estimate <- a * (1 - 0.25)^-(1 / theta_hat)
true_quantile <- a * (1 - 0.25)^-(1 / true_theta)

absolute_deviation <- abs(quantile_estimate - true_quantile)
rounded_absolute_deviation <- round(absolute_deviation, 4)
print(rounded_absolute_deviation)