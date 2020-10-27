#include<iostream>
#include <algorithm>
#include <cassert>
#include <cmath>


struct Elem {
	unsigned long long Key;
	unsigned long long Value;
};


const unsigned int BIT_SIZE = sizeof(unsigned long long) * 8;//64 áèòà
const unsigned int PART_SIZE = (unsigned int)log2(BIT_SIZE);//6 áèò
const unsigned int PART_COUNT = (BIT_SIZE / PART_SIZE) + 1;//12 øòóê - ìàêñ êîë-âî ðàçðÿäîâ
const unsigned long long PART_MASK = (unsigned long long)pow(2, PART_SIZE); //ìàêñ êëþ÷

template <typename T>
class Vector {
public:
	Vector() :
		already_used_(0), storage_size_(0), storage_(nullptr)
	{
	}

	Vector(int size, const T& default_value = T()) :
		Vector()
	{
		if (size == 0)
			return;

		already_used_ = size;
		storage_size_ = size;
		storage_ = new T[size];

		std::fill(storage_, storage_ + already_used_, default_value);
	}

	int Size() const
	{
		return already_used_;
	}

	friend void swap(Vector& lhs, Vector& rhs)
	{
		using std::swap;

		swap(lhs.already_used_, rhs.already_used_);
		swap(lhs.storage_size_, rhs.storage_size_);
		swap(lhs.storage_, rhs.storage_);
	}

	Vector& operator=(Vector other)
	{
		swap(*this, other);
		return *this;
	}


	~Vector()
	{
		delete[] storage_;

		storage_size_ = 0;
		already_used_ = 0;
		storage_ = nullptr;
	}

	void PushBack(const T& value)
	{
		if (already_used_ < storage_size_) {
			storage_[already_used_] = value;
			++already_used_;
			return;
		}

		int next_size = 1;
		if (storage_size_)
			next_size = storage_size_ * 2;

		Vector next(next_size);
		next.already_used_ = already_used_;

		if (storage_)
			std::copy(storage_, storage_ + storage_size_, next.storage_);

		next.PushBack(value);
		swap(*this, next);
	}

	const T& at(int index) const
	{
		if (index < 0 || index > already_used_)
			throw std::out_of_range("You are doing this wrong!");

		return storage_[index];
	}

	T& at(int index)
	{
		const T& Elem = const_cast<const Vector*>(this)->at(index);
		return const_cast<T&>(Elem);
	}

	const T& operator[](int index) const
	{
		return at(index);
	}

	T& operator[](int index)
	{
		return at(index);
	}

private:
	int already_used_;
	int storage_size_;
	T* storage_;
};


void CountingSortBit(Vector<Elem>& v, int size, int point)
{
	unsigned int* c = new unsigned int[PART_MASK];
	for (unsigned int i = 0; i < PART_MASK; i++) {
		c[i] = 0;
	}
	for (int i = 0; i < size; i++) {
		c[((v[i].Key) & ((PART_MASK - 1) << point)) >> point]++; 
	}
	for (unsigned int i = 1; i < PART_MASK; i++) {
		c[i] += c[i - 1];
	}
	Vector<Elem> b = Vector<Elem>(size);
	for (int i = size - 1; i >= 0; i--) {
		c[((v[i].Key) & ((PART_MASK - 1) << point)) >> point]--;
		b[c[((v[i].Key) & ((PART_MASK - 1) << point)) >> point]] = v[i];
	}
	for (int i = 0; i < size; i++) {
		v[i] = b[i];
	}
	delete[] c;
}
void RadixSort(Vector<Elem>& v) {

	for (unsigned int i = 0; i < PART_COUNT; ++i) {
		CountingSortBit(v, v.Size(), i * PART_SIZE);
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	Elem el;
	Vector <Elem> v;
	while (std::cin >> el.Key >> el.Value) {
		v.PushBack(el);
	}
	RadixSort(v);
	for (int i = 0; i < v.Size(); i++) {
		std::cout << v[i].Key << '\t' << v[i].Value << std::endl;
	}

	return 0;
}
