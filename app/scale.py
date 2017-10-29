def scale(n, min, max):
    return int(min + (max - min + 1.0) * (n / (1<<31)))
