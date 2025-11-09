#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;


class Vehicle {
protected:
    string vehicleNumber;
    string vehicleType;
    time_t entryTime;

public:
    Vehicle(string number, string type) {
        vehicleNumber = number;
        vehicleType = type;
        entryTime = time(nullptr); 
    }

    virtual double calculateFee(time_t exitTime) = 0; 

    string getNumber() const { return vehicleNumber; }
    string getType() const { return vehicleType; }
    time_t getEntryTime() const { return entryTime; }

    virtual void display() {
        cout << "Vehicle No: " << vehicleNumber
             << " | Type: " << vehicleType << endl;
    }

    virtual ~Vehicle() {}
};


class Car : public Vehicle {
public:
    Car(string number) : Vehicle(number, "Car") {}

    double calculateFee(time_t exitTime) override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return 20 * hours; // ₹20 per hour
    }
};

class Bike : public Vehicle {
public:
    Bike(string number) : Vehicle(number, "Bike") {}

    double calculateFee(time_t exitTime) override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return 10 * hours; // ₹10 per hour
    }
};


class ParkingSlot {
    int slotId;
    bool occupied;
    Vehicle* vehicle;

public:
    ParkingSlot(int id) : slotId(id), occupied(false), vehicle(nullptr) {}

    bool isOccupied() const { return occupied; }
    int getSlotId() const { return slotId; }

    void parkVehicle(Vehicle* v) {
        vehicle = v;
        occupied = true;
    }

    Vehicle* releaseVehicle() {
        occupied = false;
        Vehicle* temp = vehicle;
        vehicle = nullptr;
        return temp;
    }

    void displayStatus() {
        cout << "Slot " << setw(2) << slotId << ": ";
        if (occupied)
            cout << "Occupied by " << vehicle->getType()
                 << " (" << vehicle->getNumber() << ")" << endl;
        else
            cout << "Available" << endl;
    }
};


class ParkingLot {
    vector<ParkingSlot> slots;

public:
    ParkingLot(int totalSlots) {
        for (int i = 1; i <= totalSlots; ++i)
            slots.emplace_back(i);
    }

    int allocateSlot(Vehicle* v) {
        for (auto& slot : slots) {
            if (!slot.isOccupied()) {
                slot.parkVehicle(v);
                cout << v->getType() << " parked at Slot " << slot.getSlotId() << endl;
                return slot.getSlotId();
            }
        }
        cout << "No available slot!" << endl;
        return -1;
    }

    void releaseSlot(int slotId) {
        if (slotId <= 0 || slotId > slots.size()) {
            cout << "Invalid slot!" << endl;
            return;
        }
        ParkingSlot& slot = slots[slotId - 1];
        if (!slot.isOccupied()) {
            cout << "Slot already empty!" << endl;
            return;
        }

        Vehicle* v = slot.releaseVehicle();
        time_t exitTime = time(nullptr);
        double fee = v->calculateFee(exitTime);

        cout << "\n--- Vehicle Exited ---\n";
        v->display();
        cout << "Duration: " << fixed << setprecision(2)
             << difftime(exitTime, v->getEntryTime()) / 60 << " minutes\n";
        cout << "Total Fee: ₹" << fixed << setprecision(2) << fee << endl;

        delete v;
    }

    void showStatus() {
        cout << "\n--- Parking Lot Status ---\n";
        for (auto& slot : slots)
            slot.displayStatus();
    }
};


int main() {
    ParkingLot lot(4); 

    cout << "\n=== SMART PARKING LOT SYSTEM DEMO ===\n";

    Vehicle* v1 = new Car("OD02A1234");
    Vehicle* v2 = new Bike("OD33B9876");

    lot.allocateSlot(v1);
    lot.allocateSlot(v2);

    lot.showStatus();

    cout << "\nSimulating time delay... (3 seconds)\n";
    _sleep(3000); 

    lot.releaseSlot(1); 
    lot.showStatus();

    return 0;
}
