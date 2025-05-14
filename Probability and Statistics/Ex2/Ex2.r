library(ggplot2)

dados <- read.csv("Ex2/master.csv")
dados_filtrados <- dados[dados$year == "2002" & dados$age == "55-74 years", ]

plot <- ggplot(dados_filtrados, aes(x = sex, y = suicides.100k.pop)) +
  geom_boxplot() +
  labs(title = "Suicides per 100k population (55-74 years, 2002)",
       x = "Sex",
       y = "Suicides per 100k population")

ggsave("Ex2/plot.png", plot)