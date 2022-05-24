#include "Trainer.h"
#include "Customer.h"
#include "iostream" // DELETE: delete later

Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false), customersList(std::vector<Customer *>()),
                                   orderList(std::vector<OrderPair>()) {};

int Trainer::getCapacity() const {
    return capacity;
}

bool Trainer::isOpen() const {
    return open;
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    for (int i = customersList.size() - 1; i > -1; i--) {
        delete customersList[i];
        customersList.erase(customersList.begin() + i);
    }
    open = false;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    Customer *currCustomer = getCustomer(customer_id);
    for (int workoutId: workout_ids) {
        std::cout << currCustomer->getName() + " Is Doing " + workout_options[workoutId].getName()
                  << std::endl;
        if (!isWorkoutExists(customer_id, workout_options[workoutId])) // TODO: forum - this line
            addOrderToList(OrderPair(customer_id, workout_options[workoutId]));
    }
}

Customer *Trainer::getCustomer(int id) {
    for (Customer *customer: customersList) {
        if (customer->getId() == id)
            return customer;
    }
    return nullptr;
}

void Trainer::removeCustomer(int id) {
    for (int i = 0; i < int(customersList.size()); i++) {
        if (customersList[i]->getId() == id) {
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    if (customersList.empty())
        closeTrainer();
}

int Trainer::getSalary() const {
    int salary = 0;
    for (OrderPair order: orderList)
        salary += order.second.getPrice();
    return salary;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void Trainer::addOrderToList(OrderPair orderPair) {
    orderList.push_back(orderPair);
}

void Trainer::deleteOrdersByCustomer(int customerId) {
    std::vector<OrderPair> newOrderList = std::vector<OrderPair>();
    for (OrderPair order: orderList) {
        if (order.first != customerId)
            newOrderList.push_back(order);
    }
    orderList.clear();
    for (OrderPair order: newOrderList)
        orderList.push_back(order);
}

Trainer::~Trainer() {
    for (int i = 0; i < int(customersList.size()); i++) {
        delete customersList[i];
    }
    customersList.clear();
}

void
Trainer::copyTrainer(const bool otherOpen, const int otherCapacity, const std::vector<Customer *> &otherCustomersList,
                     const std::vector<OrderPair> &otherOrderList) {
    open = otherOpen;
    capacity = otherCapacity;
    // TODO: check if there is no memory leak - zliga
    customersList.clear();
    for (Customer *currCustomer: otherCustomersList) {
        customersList.push_back(currCustomer->clone());
    }
    orderList.clear();
    for (OrderPair pair: otherOrderList) {
        orderList.push_back(OrderPair(pair.first, pair.second));
    }
}

//TODO : forum
bool Trainer::isWorkoutExists(int id, Workout workout) {
    for (OrderPair pair: orderList)
        if (pair.first == id && pair.second.getId() == workout.getId())
            return true;
    return false;
}


// copy constructor:
Trainer::Trainer(const Trainer &other) : capacity(other.capacity), open(other.open) {
    copyTrainer(other.open, other.capacity, other.customersList, other.orderList);
}

// move constructor:
Trainer::Trainer(Trainer &&other) : capacity(other.capacity), open(other.open) {
    customersList = std::move(other.customersList);
    orderList = std::move(other.orderList);
}

// copy assignment
Trainer &Trainer::operator=(const Trainer &other) {
    if (&other != this) {
        copyTrainer(other.open, other.capacity, other.customersList, other.orderList);
    }
    return *this;
}

// move assignment
Trainer &Trainer::operator=(Trainer &&other) {
    if (this != &other) {
        open = other.open;
        capacity = other.capacity;
        customersList = std::move(other.customersList);
        orderList = std::move(other.orderList);
    }
    return *this;
}

