set.seed(2255)

# Counters
total_trials <- 150
system_no_shutdown <- 0
system_sound_alert_no_shutdown <- 0

for (i in 1:total_trials) {

    # Simulate the signals from the 9 circuits
    signals <- replicate(9, sample(1:10, 1, prob = c(1:10) / 55))

    # Check if a system shutdown was produced
    if (!(1 %in% signals)) {
        system_no_shutdown <- system_no_shutdown + 1
    }

    # Check if a sound alert was produced and the system was not shut down
    if (2 %in% signals && !(1 %in% signals)) {
        system_sound_alert_no_shutdown <- system_sound_alert_no_shutdown + 1
    }
}

proportion <- system_sound_alert_no_shutdown / system_no_shutdown
print(proportion)