# tracker.py

class Tracker:
    def __init__(self):
        # Initialize counter to 0
        self.count = 0

    def increment(self):
        """Increase the counter by 1."""
        self.count += 1

    def save_to_file(self, filename="data.txt"):
        """Write the current count to a file."""
        try:
            with open(filename, "w") as file:
                file.write(str(self.count))
        except Exception as e:
            print(f"Error saving to file: {e}")

    def __str__(self):
        """Return a human-readable representation of the current count."""
        return f"Current count: {self.count}"

