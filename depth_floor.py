# Use a GUI backend so the graph can open in a window
import matplotlib
matplotlib.use("TkAgg")

# Import the libraries we need
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Read the depth data from the TSV file
# The file has two columns: Point and Depth (m)
df = pd.read_csv("depth_data.tsv", sep="\t")

# Convert the Depth column to numbers and remove bad values
# errors="coerce" turns invalid entries like #VALUE! into NaN
# We also remove 0 and missing values because they are bad sensor readings
depth = pd.to_numeric(df["Depth (m)"], errors="coerce")
depth = depth[(depth != 0) & depth.notna()]

# Smooth the data slightly to reduce random sensor noise
# A rolling median helps remove sudden spikes without losing the main pattern
depth = depth.interpolate().rolling(5, center=True, min_periods=1).median()

# Create the graph space
fig, ax = plt.subplots(figsize=(11, 6))
fig.patch.set_facecolor("#f7f9fc")
ax.set_facecolor("#f7f9fc")

# Add title and axis labels
ax.set_title("Ship Depth Over Time", fontsize=14, fontweight="bold")
ax.set_xlabel("Time (s)", fontsize=11)
ax.set_ylabel("Depth (m)", fontsize=11)

# Make the graph look neat
ax.grid(True, alpha=0.3)
ax.set_xlim(0, len(depth) - 1)
ax.set_ylim(depth.min() - 10, depth.max() + 10)
ax.invert_yaxis()  # So deeper values appear lower on the graph

# Plot the depth line and the current moving point
line, = ax.plot([], [], color="dodgerblue", lw=2.5, label="Depth line")
point, = ax.plot([], [], "o", color="red", markersize=8, label="Current point")

# Text box that shows the current time and depth value
text = ax.text(
    0.02, 0.96, "",
    transform=ax.transAxes,
    fontsize=11,
    bbox=dict(boxstyle="round,pad=0.4", facecolor="white", edgecolor="gray", alpha=0.9)
)

# Add legend
ax.legend(loc="upper right", fontsize=10)

# This function runs for each frame of the animation
# It draws the line up to the current time and shows the value at that moment
def update(frame):
    x = list(range(frame + 1))
    y = depth.iloc[:frame + 1].tolist()

    line.set_data(x, y)
    point.set_data([frame], [depth.iloc[frame]])
    text.set_text(f"Time = {frame}s   Depth = {depth.iloc[frame]:.2f} m")
    return line, point, text

# Create the animation object
anim = FuncAnimation(fig, update, frames=len(depth), interval=100, blit=False)

# Save a still image too
fig.savefig("depth_plot.png", dpi=200)

# Show the graph window
plt.tight_layout()
plt.show()
