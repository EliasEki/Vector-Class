#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<stdexcept>
#include<initializer_list>
using namespace std;

template<typename T>
class Vector{
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;
private:
	size_t sz;
	size_t max_sz;
	value_type* values;

	void allocate(size_t new_max_sz){ //function, um neuen speicher zu allozieren, sollte man mehr/weniger brauchen
		value_type* help {new value_type[new_max_sz]};
		for(size_t i{0}; i<this->sz; i++)
			help[i] = this->values[i];
		delete[] this->values;
		this->values = help;
		max_sz = new_max_sz;
	}

public:

	Vector(size_t n=5): sz{0}, max_sz{n}, values{new value_type [max_sz]}{} // reservieren einen array der größe n, mit n=4 ist jetzt defaultkonstruktor, damit der vector automatisch größe 4 hat, wenn man keine geöße übergibt
	// in this first contrucor, absolutely dont add excepion when max_sz is >= 0, this has to be allowed for utest to work
	
	~Vector(){delete[] this->values;}
	
	//kopierkonstruktor:
	Vector(const Vector& other): sz{other.sz}, max_sz{other.max_sz}, values{new value_type[max_sz]}{
		for(size_t i{0}; i<sz; ++i){
			values[i] = other.values[i];
		}
	}
	
	//für initialisieren einer liste:
	Vector(std::initializer_list<value_type> li):Vector(li.size()){
		for(const auto& x : li){
			this->push_back(x);
		}
	}
	
	size_t size() const{
		return sz;// returniert die size von dem this object
	}
	
	bool empty() const{
		return sz==0;
	}
	
	void clear(){
		sz=0;
	}

	void reserve(size_t n){
		if(n<max_sz)
			return;
		allocate(n);
	}
	
	void shrink_to_fit(){
		allocate(sz);
	}
			
	void push_back(value_type x){//only push, if there is space left
		if(this->max_sz == this->sz){
			this->allocate((2 * this->sz) + 1); // +1 in case the 
		}

		this->values[this->sz] = x;
		++this->sz;
		//oder optional in einer Zeile values[sz++] = x;
	}

	void pop_back(){
		if(sz==0)
			throw std::runtime_error("Empty vector.");
		sz--;
	}
		
	value_type& operator[](size_t index){
		if(index >= sz){
			throw std::runtime_error("Out of Bounds");
		}
		return values[index];
	}
	
	const value_type& operator[](size_t index) const {
		if(index >= sz || index < 0){
			throw std::runtime_error("Out of Bounds");
		}
		return values[index];
	}
	
	Vector& operator=(Vector other) { // mit lösung von cpp.reference
		std::swap(sz, other.sz);
		std::swap(max_sz, other.max_sz);
		std::swap(values, other.values);
		return *this;
	}

	size_t capacity() const{
		return max_sz;
	}

	
	friend std::ostream& operator<<(std::ostream& os, const Vector& v){
		os << "[";
		bool first{true};
		for(size_t i{0}; i<v.sz; ++i){
			if(first)
				first=false;
			else
				os << ", ";
			os << v[i];
		}
		return os << "]";
	}

    iterator begin(){ // retourniert einen Iterator der auf das erste Element im Vector zeigt, wenn vector leer, dann ist das der end-iterator
		// dazu mache ich einen iterator aus dem pointer von der instanzvariable, und im alias haben wir ja definiert, iterator = Vector::Iterator
		return iterator(this->values, this->values, this->values + this->sz);
	}

    iterator end(){ // soll einen iterator retornieren der auf eins nach dem letzten element verweist, die variable sz, ist ja immer genau das, sz=0 wenn leer, und sz=5 wenn der vector 5 elemente hat, wenn ich v[5] referenziere, dann greife ich aber auf ein element zu weit weg zu, daher dereferenzieren wir einfach das erste element + sz slots weiter rechts:
		return iterator(this->values + this->sz, this->values, this->values + this->sz);
	}

    const_iterator begin() const{
		return const_iterator(this->values, this->values, this->values + this->sz);
	}

    const_iterator end() const{
		return const_iterator(this->values + this->sz, this->values, this->values + this->sz);
	}
	
    class Iterator {
        public:
            using value_type = Vector::value_type;
            using reference = Vector::reference;
            using pointer = Vector::pointer;
            using difference_type = Vector::difference_type;
            using iterator_category = std::forward_iterator_tag;
        private:
            pointer ptr; // oder value_type* ptr, ist wegen alias dasselbe...
			pointer anfang; // als tracker für Secure-region
			pointer schluss; // --''--
			// schluss und anfang sind nullptr, wenn unbekannt wo ende und anfang von meinem vrector ist, und v.begin() und v.end()sonst
			// natürlich begin und end in der vector klasse updaten
        public:
        
		Iterator(): ptr{nullptr}, anfang{nullptr}, schluss{nullptr} {}

		Iterator(pointer other_ptr, pointer start=nullptr, pointer ende=nullptr): ptr{other_ptr}, anfang{start}, schluss{ende} {}

		reference operator*() const{ // reference = value_type&, also eine adresse von dem wert, auf den der iterator verweist
			if(this->ptr == nullptr){
				throw runtime_error("Cant dereference null");
			}
			if(this->anfang == nullptr || this->schluss == nullptr){
				throw runtime_error("Cant dereference in unknown range");
			}
			if(this->ptr < this->anfang || this->ptr >= this->schluss){
				throw runtime_error("Cant dereference outside of range");
			}

			return *(this->ptr);
		}

		pointer operator->() const{
			if(this->ptr == nullptr){
				throw runtime_error("Cant dereference null");
			}
			if(this->anfang == nullptr || this->schluss == nullptr){
				throw runtime_error("Cant dereference in unknown range");
			}
			if(this->ptr < this->anfang || this->ptr >= this->schluss){
				throw runtime_error("Cant dereference outside of range");
			}

			return this->ptr;
		}

		friend bool operator==(const iterator& lop, const iterator& rop){
			return (lop.ptr == rop.ptr);
		}
		friend bool operator!=(const iterator& lop, const iterator& rop){
			return (lop.ptr != rop.ptr);
		}

		iterator& operator++(){// das ist Prefix, und man soll eine referenz auf den iterator t#retournieren, nicht auf den pointer
			if(this->schluss != nullptr && this->anfang != nullptr){// also sind wir schonmal nicht in undefiniertem bereich
				if(this->ptr >= this->anfang && this->ptr < this->schluss){// also sind wir in richtiger range, und haben das ende noch nicht erreicht
					(this->ptr)++; // optional ++ptr;
				}
				else{} // wenn wir bei end sind(ptr == schluss), dann machen wir nichts, wir schalten nicht weiter
			}
			else{} // wenn wir in nicht definierten bereich sind, püassiert nichts

			return *(this);
		}
		iterator operator++(int){// das ist Postfix, muss sich also den wert merken,
			iterator copy_iter = *this;
			// jetzt das prefix von oben nutzen:
			++(*this);
			return copy_iter;
		}
		// für die -- version muss man den zweiten check ändern ptr > anfang && ptr <= schluss

		operator const_iterator() const{
			return const_iterator(ptr, anfang, schluss);
		}; // typ konversion auf const_iterator

		friend Vector::difference_type operator-(const iterator& lop, const iterator& rop){ // zu glob function machen, macht differenz mit anderem const_iterator
			return static_cast<Vector::difference_type>(lop.ptr - rop.ptr);
		}

    };

    class ConstIterator {
        public:
            using value_type = Vector::value_type;
            using reference = Vector::const_reference;
            using pointer = Vector::const_pointer;
            using difference_type = Vector::difference_type;
            using iterator_category = std::forward_iterator_tag;
        private:
			pointer ptr;
			pointer anfang;
			pointer schluss;

        public:
        
		ConstIterator(): ptr{nullptr}, anfang{nullptr}, schluss{nullptr} {}

		ConstIterator(pointer other_ptr, pointer start=nullptr, pointer ende=nullptr): ptr{other_ptr}, anfang{start}, schluss{ende} {}

		reference operator*() const{ // reference = value_type&, also eine adresse von dem wert, auf den der iterator verweist
			if(this->ptr == nullptr){
				throw runtime_error("Cant dereference null");
			}
			if(this->anfang == nullptr || this->schluss == nullptr){
				throw runtime_error("Cant dereference in unknown range");
			}
			if(this->ptr < this->anfang || this->ptr >= this->schluss){
				throw runtime_error("Cant dereference outside of range");
			}
				return *(this->ptr);
		}
		pointer operator->() const{
			if(this->ptr == nullptr){
				throw runtime_error("Cant dereference null");
			}
			if(this->anfang == nullptr || this->schluss == nullptr){
				throw runtime_error("Cant dereference in unknown range");
			}
			if(this->ptr < this->anfang || this->ptr >= this->schluss){
				throw runtime_error("Cant dereference outside of range");
			}
			return this->ptr;
		}

		friend bool operator==(const const_iterator& lop, const const_iterator& rop){// zu glob function machen
			return (lop.ptr == rop.ptr);
		}
		friend bool operator!=(const const_iterator& lop, const const_iterator& rop){// zu glob function machen
			return (lop.ptr != rop.ptr);
		}

		const_iterator& operator++(){// das ist Prefix, und man soll eine referenz auf den iterator t#retournieren, nicht auf den pointer
			if(this->schluss != nullptr && this->anfang != nullptr){// also sind wir schonmal nicht in undefiniertem bereich
				if(this->ptr >= this->anfang && this->ptr < this->schluss){// also sind wir in richtiger range, und haben das ende noch nicht erreicht
					(this->ptr)++; // optional ++ptr;
				}
				else{} // wenn wir bei end sind(ptr == schluss), dann machen wir nichts, wir schalten nicht weiter
			}
			else{} // wenn wir in nicht definierten bereich sind, püassiert nichts

			return *(this);
		}
		const_iterator operator++(int){// das ist Postfix, muss sich also den wert merken, INT BLEIBT, einfach, damit compiler weiß, was dasses die Postfix version ist, isses immer int? oser soll zu value_type
			const_iterator copy_iter = *this;
			// jetzt das prefix von oben nutzen:
			++(*this);
			return copy_iter;
		}

		friend Vector::difference_type operator-(const const_iterator& lop, const const_iterator& rop){ // zu glob function machen, macht differenz mit anderem const_iterator
			return static_cast<Vector::difference_type>(lop.ptr - rop.ptr);
		}
		//wenn man global will, einfach friend davor, aber dann gibt es sozusagen kein this objekt mehr, mit dem ich arbeiten kann, dann braucht man zwei variablen, ansonsten würde man mit this und other vergleichen

    };

	iterator insert(const_iterator pos, const_reference val) {
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) > sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)};
		if (sz >= max_sz)
			reserve(max_sz * 2 + 1); // Achtung Sonderfall, wenn keine Mindestgroesze definiert ist
		for (auto i{sz}; i-- > current;)
			values[i + 1] = values[i];
		values[current] = val;
		++sz;
		return iterator{values + current, this->values, this->values + this->sz}; // vorher: iterator{values + current}, aber das macht trouble wen das retourn object nutzen will
	}

	iterator erase(const_iterator pos) {
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)};
		for (auto i{current}; i < sz - 1; ++i)
			values[i] = values[i + 1];
		--sz;
		return iterator{values + current, this->values, this->values + this->sz}; // vorher: iterator{values + current}, aber das macht trouble wen das retourn object nutzen will
	}

};
#endif
