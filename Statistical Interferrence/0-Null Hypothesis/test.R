# analysis.R – simple demo
message("Hello from Visual Studio + RTVS!")

# Create a small data frame
df <- data.frame(
  x = 1:10,
  y = rnorm(10)
)

# Plot the data (the plot appears in the RTVS Plot window)
library(ggplot2)
ggplot(df, aes(x, y)) + geom_point()