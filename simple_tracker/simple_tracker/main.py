# main.py

import time
from tracker import Tracker

def read_interval_from_config(filepath="config/config.txt"):
    """Read and return the interval value from the config file."""
    try:
        with open(filepath, "r") as file:
            for line in file:
                if line.lower().startswith("interval"):
                    key, value = line.strip().split("=")
                    return int(value.strip())
    except FileNotFoundError:
        print("Error: config/config.txt not found.")
    except Exception as e:
        print(f"Error reading config: {e}")

    # Default to 5 seconds if not found or invalid
    return 5


if __name__ == "__main__":
    interval = read_interval_from_config()
    tracker = Tracker()

    print(f"Starting tracker with interval = {interval} seconds")

    while True:
        tracker.increment()
        print(tracker)
        tracker.save_to_file()
        time.sleep(interval)
