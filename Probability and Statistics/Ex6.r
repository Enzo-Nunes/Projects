set.seed(1973)

sample_size <- 40
a <- 4
threshold <- 126
num_samples <- 1000

gamma_shape <- 40
gamma_rate <- 1 / 4

# Generate samples from an exponential distribution
samples <- replicate(num_samples, rexp(sample_size, 1 / a))

# Calculate the sum of each sample
y <- colSums(samples)

simulated_p <- sum(y > threshold) / num_samples
theoretical_p <- 1 - pgamma(threshold, shape = gamma_shape, rate = gamma_rate)
abs_difference <- abs(simulated_p - theoretical_p)
result <- round(abs_difference * 100, 5)
print(result)
