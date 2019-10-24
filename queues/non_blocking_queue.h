#include "../common/allocator.h"

#define LFENCE asm volatile("lfence" : : : "memory")
#define SFENCE asm volatile("sfence" : : : "memory")

template<class P>
struct pointer_t {
    P* ptr;   // pointer to the node

    P* address(){
        return reinterpret_cast<P*>(reinterpret_cast<uintptr_t>(ptr) & static_cast<uintptr_t>(0xFFFFFFFFFFFF));
        // Get the address by getting the 48 least significant bits of ptr
    }
    uint count(){
        return static_cast<uint>((reinterpret_cast<uintptr_t>(ptr) & static_cast<uintptr_t>(0xFFFF000000000000)) >> 48);
        // Get the count from the 16 most significant bits of ptr
    }
    void setPtr(P* address, uint count){
        ptr = reinterpret_cast<P*>(reinterpret_cast<uintptr_t>(address) & (static_cast<uintptr_t>(count) << 48));
    }
};

template<class T>
struct Node {
    T value;
    pointer_t<Node<T>> next;
};

template<class T>
struct NonBlockingQueue {
    pointer_t<Node<T>> q_head;
    pointer_t<Node<T>> q_tail;
    CustomAllocator my_allocator;

    NonBlockingQueue() : my_allocator()
    {
        std::cout << "Using NonBlockingQueue\n";
    }
    
    void initQueue(long allocator_init) {
        my_allocator.initialize(allocator_init, sizeof(Node<T>));
        Node<T>* node = (Node<T>* )my_allocator.newNode();
        node->next.ptr = NULL;
        q_head.ptr = q_tail.ptr = node;
    }

    void enqueue(T value) {
        Node<T>* node = (Node<T>* )my_allocator.newNode();
        node->value = value;
        node->next.ptr = NULL;
        SFENCE;
        pointer_t<Node<T>> tail;
        pointer_t<Node<T>> next;
        pointer_t<Node<T>> newVal;
        while(true) {
            tail = q_tail;
            LFENCE;
            next = tail.address()->next;
            LFENCE;
            if (tail.ptr == q_tail.ptr){
                if (next.address() == NULL) {
                    newVal.setPtr(node, next.count()+1);
                    if(CAS(&tail.address()->next, next, newVal))
                        break;
                }
                else
                    newVal.setPtr(next.address(), tail.count()+1);
                    CAS(&q_tail, tail, newVal);    // ELABEL
            }
        }
        SFENCE;
        newVal.setPtr(node, tail.count()+1);
        CAS(&q_tail, tail, newVal);
    }

    bool dequeue(T* p_value) {
        pointer_t<Node<T>> next;
        pointer_t<Node<T>> head;
        pointer_t<Node<T>> tail;
        pointer_t<Node<T>> newVal;
        while(true){
            head = q_head;
            LFENCE;
            tail = q_tail;
            LFENCE;
            next = head.address()->next;
            LFENCE;
            if (head.ptr == q_head.ptr) {
                if(head.address() == tail.address()) {
                    if(next.address() == NULL)
                            return false;
                    newVal.setPtr(next.address(), tail.count()+1);
                    CAS(&q_tail, tail, newVal);    //DLABEL
                }
                else {
                    *p_value = next.address()->value;
                    newVal.setPtr(next.address(), head.count()+1);
                    if(CAS(&q_head, head, newVal))
                        break;
                }
            }
        }
        my_allocator.freeNode(head.address());
        return true;
    }
    void cleanup() {
        my_allocator.cleanup();
    }
};