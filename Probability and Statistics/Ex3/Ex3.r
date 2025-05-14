library(readxl)
library(ggplot2)

data <- read_excel("Ex3/electricity.xlsx", col_types = "text")
data$share <- as.numeric(data$share)

# Convert YEAR and MONTH to date format
data$DATE <- as.Date(paste(data$YEAR, data$MONTH, "01", sep = "-"), format = "%Y-%m-%d")

# Filter the data for the countries and the years we are interested in
filtered_data <- data[data$COUNTRY %in% c("Italy", "Latvia", "IEA Total") &
as.numeric(format(data$DATE, "%Y")) >= 2015 & data$PRODUCT == "Renewables", ]

filtered_data$percentage <- filtered_data$share * 100

plot <- ggplot(filtered_data, aes(x = DATE, y = percentage, color = COUNTRY)) +
    geom_line() +
    scale_y_continuous(limits = c(0, 100)) +
    labs(x = "Date", y = "Renewable Energy Production (%)",
        title = "Evolution of Renewable Energy Production (2015-2022)",
        color = "Country")

ggsave("Ex3/plot.png", plot)