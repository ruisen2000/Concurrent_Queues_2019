#include "../common/allocator.h"
#include <mutex>

template <class T>
struct Node
{
    T value;
    Node<T>* next;
};

template <class T>
class TwoLockQueue
{
private:
    Node<T>* q_head;
    Node<T>* q_tail;

    CustomAllocator my_allocator_;
    std::mutex enq_lock;
    std::mutex deq_lock;

public:
    TwoLockQueue() : my_allocator_()
    {
        std::cout << "Using TwoLockQueue\n";
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        // Initialize the queue head or tail here
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        q_head = q_tail = newNode;  // sentinal node
    }

    void enqueue(T value)
    {
        enq_lock.lock();
        Node<T>* node = (Node<T>* )my_allocator_.newNode();
        node->value = value;
        node->next = NULL;

        q_tail->next = node;
        q_tail = node;
        enq_lock.unlock();
    }

    bool dequeue(T *value)
    {
        deq_lock.lock();
        Node<T>* node = q_head;
        Node<T>* new_head = q_head->next;

        if(new_head == NULL){
            // Queue is empty
            deq_lock.unlock();
            return false;
        }

        *value = new_head->value;
        q_head = new_head;
        deq_lock.unlock();
        my_allocator_.freeNode(node);

        return true;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};