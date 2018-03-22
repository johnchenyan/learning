#include <thread>
#include <functional>
#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <mutex>

template <typename T>
class priority_queue
{
public:
	explicit
	priority_queue(unsigned int capacity);
	priority_queue(priority_queue const & other);
	auto operator = (priority_queue const & other) -> priority_queue &;
	~priority_queue();

	auto empty() const ->bool;
	auto size() const ->size_t;

	auto enqueue(T const & value) ->bool;
	auto dequeue(T& value) ->bool;

	void print();

private:
	unsigned int capacity_;
	unsigned int size_;
	T*           heap_;

	mutable std::mutex mutexpriority_queue_;

private:
	auto throw_is_empty() const -> void;
	void filte_up(int index);
	void filte_down(int index);
};

template<typename T>
auto priority_queue<T>::throw_is_empty() const -> void
{
	throw std::logic_error("Error");
}

template<typename T>
priority_queue<T>::priority_queue(unsigned int capacity) : capacity_(capacity), size_(0)
{
	heap_ = (T*)malloc(sizeof(T)*capacity_);
}

template <typename T>
priority_queue<T>::priority_queue(priority_queue const & st) : mutexpriority_queue_()
{

}

template <typename T>
priority_queue<T>::~priority_queue()
{
	delete heap_;
	heap_ = nullptr;
}

template <typename T>
auto priority_queue<T>::operator=(const priority_queue & st) ->priority_queue &
{
	return *this;
}

template <typename T>
void priority_queue<T>::print()
{
	for(int i = 0; i < size_; ++i)
	{
		std::cout << heap_[i]<<" ";
	}
	std::cout << "\n";
}

template<typename T>
size_t priority_queue<T>::size() const
{
	std::lock_guard<std::mutex> lock(mutexpriority_queue_);
	return size_;
}

template <typename T>
auto priority_queue<T>::enqueue(T const &value)->bool
{
	std::lock_guard<std::mutex> lock(mutexpriority_queue_);
	if(size_ == capacity_)
	{
		return false;
	}
	heap_[size_] = value;
	filte_up(size_);
	size_++;
	return true;
}

template <typename T>
void priority_queue<T>::filte_up(int index)
{
	T value = heap_[index];
	int current_index = index;
	while(current_index > 0)
	{
		int parent_index = (current_index - 1)/2;
		if(heap_[parent_index] < value)
		{
			T parent_value = heap_[parent_index];
			heap_[parent_index] = heap_[current_index];
			heap_[current_index] = parent_value;
		}
		else
		{
			break;
		}
		current_index = parent_index;
	}
}

template <typename T>
auto priority_queue<T>::dequeue(T &value) ->bool
{
	std::lock_guard<std::mutex> lock(mutexpriority_queue_);
	if(0 == size_)
	{
		return false;
	}
	value = heap_[0];
	filte_down(size_);
	size_--;
	return true;
}

template<typename T>
void priority_queue<T>::filte_down(int index)
{
	int parent_index = 0;
	heap_[parent_index] = heap_[index];
	int child_index = parent_index * 2 + 1; //left child
	while(parent_index <= index)
	{
		if(heap_[child_index] < heap_[child_index+1])
		{
			child_index++;
		}

		if(heap_[parent_index] < heap_[child_index])
		{
			T value = heap_[parent_index];
			heap_[parent_index] = heap_[child_index];
			heap_[child_index] = value;
		}
		else
		{
			break;
		}

		parent_index = child_index;
		child_index = parent_index * 2 + 1; //left child
	}
}
