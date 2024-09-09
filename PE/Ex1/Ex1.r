library(ggplot2)

# Read the data from the CSV file and define the continents and countries of interest
data <- read.csv("Ex1/Paises_PIB_ICH.csv")
continents <- c("Asia", "Africa")
data_filtered <- data[data$Continent %in% continents, ]
specific_countries <- c("United Arab Emirates", "Nepal", "Comoros", "Namibia")
data_filtered$label <- ifelse(data_filtered$Country %in% specific_countries, as.character(data_filtered$Country), NA)

# Create the scatter plot
plot <- ggplot(data_filtered, aes(x = GDP, y = HCI, color = Continent)) +
	scale_x_log10() +
    geom_point(shape = 18) +
    geom_point(data = data_filtered[data_filtered$Country %in% specific_countries, ], shape = 18, color = "black") +
    geom_text(aes(label = label), vjust = 1, hjust = 1, na.rm = TRUE, show.legend = FALSE) +
    labs(
        x = "GDP per Capita",
        y = "Human Capital Index",
        title = "HCI in function of the GDP per Capita for Asia and Africa",
        color = "Continent"
    ) +
    theme_grey()

ggsave("plot.png", plot)