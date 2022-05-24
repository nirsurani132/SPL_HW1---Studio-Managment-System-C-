#ifndef WORKOUT_H_
#define WORKOUT_H_

#include <string>

enum WorkoutType{
    ANAEROBIC, MIXED, CARDIO
};

class Workout{
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    WorkoutType getType() const;
    friend std::ostream& operator<<(std::ostream& os, const Workout& workout);
    static std::string workoutTypeToString(WorkoutType type);
    Workout(const Workout &other);
    Workout(Workout &&other);
    ~Workout();
private:
	const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};

// TODO: DElete:
//class WorkoutIdComperator {
//    inline bool operator() (const Workout& workout1, const Workout& workout)
//    {
//        return (workout1.getPrice() > workout.getPrice());
//    }
//};

bool compareWorkoutPricePairs (const std::pair<int,int> workout1, const std::pair<int,int> workout2);

#endif