import matplotlib.pyplot as plt

def plot_polyline_from_file(path):
    points = []

    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            # accetta virgola o punto
            sep = ',' if ',' in line else '.'
            x_str, y_str = line.split(sep)
            points.append((float(x_str), float(y_str)))

    xs = [p[0] for p in points]
    ys = [p[1] for p in points]

    plt.figure()
    plt.plot(xs, ys)
    plt.title("Polyline")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.show()


# Uso:
plot_polyline_from_file("./input_test.txt")
