set.seed(2822)

type_1_errors <- 0
type_2_errors <- 0

for (i in 1:5000) {
    sample_h0 <- rpois(100, 2.90)
    sample_11 <- rpois(100, 3.15)

    mean_h0 <- mean(sample_h0)
    mean_h1 <- mean(sample_11)

    if (mean_h0 > 3.234) {
        type_1_errors <- type_1_errors + 1
    }
    if (mean_h1 <= 3.234) {
        type_2_errors <- type_2_errors + 1
    }
}

rel_freq_type_1 <- type_1_errors / 5000
rel_freq_type_2 <- type_2_errors / 5000
error_ratio <- rel_freq_type_2 / rel_freq_type_1
print(error_ratio)