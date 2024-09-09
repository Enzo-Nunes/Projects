set.seed(1950)

n <- 23
r <- 300
m <- 170

# Define the function to generate t-distributed values
generate_t <- function(n) {
    z <- rnorm(n + 1)
    sqrt(n) * z[1] / sqrt(sum(z[-1]^2))
}

samples <- replicate(r, replicate(m, generate_t(n)))

# Calculate the proportion of values <= 1.5 for each sample
proportions <- apply(samples, 2, function(sample) mean(sample <= 1.5))

simulated_p <- mean(proportions)
theoretical_p <- pt(1.5, df = n)
abs_difference <- abs(simulated_p - theoretical_p)
result <- round(abs_difference * 100, 5)
print(result)
