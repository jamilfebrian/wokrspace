class Car:

    wheels = 29
    def __init__(self, make, model, year, makan):
        self.make = make
        self.model = model
        self.year = year
        self.makan = makan

car = Car("Honda", "jadul",2020, 2)
print(car.make)