#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    ~Trainer();
    Trainer(const Trainer& other);
    Trainer& operator=(const Trainer &other);
    Trainer& operator=(Trainer &&other);
    void copyTrainer (const bool open, const int capacity, const std::vector<Customer*>& customersList, const std::vector<OrderPair>& orderList);
    Trainer(Trainer && other);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary() const;
    bool isOpen() const;
    void addOrderToList(OrderPair orderPair);
    void deleteOrdersByCustomer(int customerId);
    bool isWorkoutExists (int id, Workout workout);
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif
