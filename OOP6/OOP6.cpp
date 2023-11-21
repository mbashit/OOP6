// OOP6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <fstream>

template <class TInfo>
struct NODE
{
	TInfo info;
	NODE* next;
	NODE* prev;
	NODE(TInfo info, NODE* next = nullptr, NODE* prev = nullptr) :info(info), next(next), prev(prev) {}
	~NODE() { next = nullptr; prev = nullptr; };
};

template <class TInfo>
class SelfOrderedLIST
{
public:
	class iterator;
private:
	NODE<TInfo>* head;
	NODE<TInfo>* tail;
	size_t size;
	iterator Head, Tail;
public:
	SelfOrderedLIST() { head = new NODE<TInfo>(0); tail = head; size = 0; Head = iterator(head); Tail = iterator(tail); };
	SelfOrderedLIST(std::ifstream& file)
	{
		TInfo elem;
		while (file >> elem)
			add_to_tail(elem);
		Head = iterator(head);
		Tail = iterator(tail);
	}
	~SelfOrderedLIST()
	{
		while (!empty())
			del_from_head();
		delete head;
	}
	NODE<TInfo>* get_head() { return head; };
	TInfo get_first() { return head->next->info; };
	size_t get_size() { return size; };
	iterator begin() { return Head; };
	iterator end() { return Tail; };
	iterator c_begin() const { return Head; };
	iterator c_end() const { return Tail; };
	bool empty()
	{
		return size == 0;
	}
	void add_to_head(TInfo elem)
	{
		if (empty())
		{
			head = new NODE<TInfo>(0);
			tail = head;
		}
		NODE<TInfo>* p = new NODE<TInfo>(elem, head->next, head);
		head->next = p;
		if (p->next)
			p->next->prev = p;
		if (head == tail)
			tail = tail->next;
		++size;
	}
	void add_to_tail(TInfo elem)
	{
		if (empty())
		{
			head = new NODE<TInfo>(0);
			tail = head;
		}
		NODE<TInfo>* p = new NODE<TInfo>(elem, tail->next, tail);
		tail = p;
		tail->prev->next = p;
		++size;
	}
	void del_from_head()
	{
		if (head->next)
		{
			NODE<TInfo>* p = head->next;
			head->next = p->next;
			if (p->next)
				p->next->prev = p->prev;
			p->next = nullptr;
			p->prev = nullptr;
			delete p;
			if (!head->next)
				tail = head;
			--size;
		}
	}
	void del_from_tail()
	{
		NODE<TInfo>* p = tail;
		tail = p->prev;
		if (p->prev)
		{
			p->prev->next = nullptr;
			p->prev = nullptr;
		}
		--size;
		delete p;
	}
	void print()
	{
		if (empty())
		{
			std::cout << "List is empty:(\n";
		}
		else
		{
			NODE<TInfo>* p = head->next;
			while (p)
			{
				std::cout << p->info << ' ';
				p = p->next;
			}
			std::cout << '\n';
		}
	}
	void print(std::ofstream& file, std::string message)
	{
		NODE<TInfo>* ptr = head->next;
		std::cout << "В файл output.txt сделана запись\n";
		file << message << '\n';
		while (ptr)
		{
			file << ptr->info << ' ';
			ptr = ptr->next;
		}
	}
	void add_by_iterator(iterator it, const TInfo& elem)
	{
		NODE<TInfo>* el = it.elem;
		if (it == end()) add_to_tail(elem);
		else
		{
			NODE<TInfo>* next_el = it.elem->next;
			NODE<TInfo>* p = new NODE<TInfo>(elem);
			p->next = next_el; p->prev = el;
			next_el->prev = p; el->next = p;
		}
		size++;
	}
	void del_by_iterator(iterator it)
	{
		NODE<TInfo>* el = it.elem;
		if (it == begin()) del_from_head();
		else if (it == end()) del_from_tail();
		else
		{
			NODE<TInfo>* prev_el = it.elem->prev;
			NODE<TInfo>* next_el = it.elem->next;
			prev_el->next = next_el;
			next_el->prev = prev_el;
			delete el;
		}
		--size;
	}
	bool search(TInfo elem, iterator& res)
	{
		NODE<TInfo>* tmp = head->next;
		while (tmp != nullptr && tmp->info != elem)
			tmp = tmp->next;
		if (tmp == nullptr)
			return false;
		res = (iterator)tmp;
		if (tmp->prev == head)
			return true;
		if (tmp->prev)
			tmp->prev->next = tmp->next;
		if (tmp->next)
			tmp->next->prev = tmp->prev;
		tmp->prev = head;
		tmp->next = head->next;
		head->next = tmp;
		return true;
	}
	class iterator
	{
		friend class SelfOrderedLIST<TInfo>;
	private:
		NODE<TInfo>* elem;
	public:
		iterator(NODE<TInfo>* elem) :elem(elem) {};
		iterator() :elem(0) {};
		iterator(const iterator& it) :elem(it.elem) {};
		bool operator==(const iterator& it) const { return (elem == it.elem); };
		bool operator!=(const iterator& it) const { return !(elem == it.elem); };
		iterator& operator++()
		{
			if (elem)
				elem = elem->next;
			return *this;
		}
		iterator& operator--()
		{
			if (elem)
				elem = elem->prev;
			return *this;
		}
		TInfo& operator*()
		{
			if (elem)
				return elem->info;
		}
		iterator& operator+=(int n)
		{
			int i = 0;
			while (i < n)
			{
				if (elem)
					elem = elem->next;
				++i;
			}
			return *this;
		}
	};
};

int main()
{
	SelfOrderedLIST<int> list;
	for (int i = 0; i < 10; i++)
		list.add_to_tail(i);
	for (int i = 0; i < 10; i++)
	{
		SelfOrderedLIST<int>::iterator tmp;
		list.search(i, tmp);
		std::cout << *tmp << "\n";
		list.print();
	}
	SelfOrderedLIST<int>::iterator tmp;
	bool f = list.search(10, tmp);
	if (f)
		std::cout << *tmp << "\n";
	list.print();
	return 0;
}
