# read_config.py

def read_interval_from_config():
    try:
        # Open and read the config file
        with open("config/config.txt", "r") as file:
            lines = file.readlines()
        
        interval = None
        # Extract the interval value
        for line in lines:
            if line.lower().startswith("interval"):
                # Expected format: interval=10 (for example)
                key, value = line.strip().split("=")
                interval = value.strip()
                break

        if interval:
            print(f"Interval value: {interval}")
        else:
            print("No interval value found in config/config.txt")

    except FileNotFoundError:
        print("Error: config/config.txt not found.")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    read_interval_from_config()
