#ifndef PQITEM_H
#define PQITEM_H

template<typename T>
class PQItem {
    private:
        T data;
        int priority;

    public:
        PQItem(T data, int priority) : data(data), priority(priority) {}

        T getData() const { return data; }

        int getPriority() const { return priority; }

        bool operator < (const PQItem &item) const { return getPriority() > item.getPriority(); }
};

#endif