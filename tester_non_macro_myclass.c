#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <typeinfo>

struct MyClass 
{
    	int id;
    	char name[10];
};

void MyClass_print(const MyClass * o) 
{
    printf("%d\n", o -> id);
    printf("%s\n", o -> name);
}

bool MyClass_less_by_id (const MyClass & o1, const MyClass & o2) 
{
    	return o1.id < o2.id;
}

struct Deque_MyClass_Iterator
{
      MyClass * iter;

      void (* inc) (Deque_MyClass_Iterator *);
      void (* dec) (Deque_MyClass_Iterator *);

      MyClass & (* deref) (Deque_MyClass_Iterator *);
};

MyClass & deref_def(Deque_MyClass_Iterator * deq_iter_ptr)
{
	return *(deq_iter_ptr -> iter);
}

void inc_def(Deque_MyClass_Iterator * deq_iter_ptr)
{
	deq_iter_ptr -> iter++;
}

void dec_def(Deque_MyClass_Iterator * deq_iter_ptr)
{
	deq_iter_ptr -> iter--;
}

bool
Deque_MyClass_Iterator_equal(Deque_MyClass_Iterator it, Deque_MyClass_Iterator end)
{
	// Returns True if the iterators point to the same MyClass object
      if (it.iter == end.iter) return true;
      else return false; 
}

struct Deque_MyClass 
{
    	MyClass * data;
    	size_t capacity;
    	size_t length;
    	size_t front_index;
    	size_t back_index;
    	char type_name[sizeof("Deque_MyClass")];

    	size_t (* size) (Deque_MyClass *);
    	bool (* empty) (Deque_MyClass *);

    	void (* push_back) (Deque_MyClass *, MyClass);
    	void (* push_front) (Deque_MyClass *, MyClass);

      MyClass & (* back) (Deque_MyClass *);
      MyClass & (* front) (Deque_MyClass *);

      void (* pop_back) (Deque_MyClass *);
      void (* pop_front) (Deque_MyClass *);

      Deque_MyClass_Iterator begin_iter;

      Deque_MyClass_Iterator (* begin) (Deque_MyClass *);
      Deque_MyClass_Iterator (* end) (Deque_MyClass *);

      void (* print) (Deque_MyClass * deq_ptr);
      MyClass & (* at) (Deque_MyClass * deq_ptr, size_t index);

      void (* clear) (Deque_MyClass * deq_ptr);
      void (* dtor) (Deque_MyClass * deq_ptr);
};

size_t size_def(Deque_MyClass * deq_ptr)
{
	return deq_ptr -> length;
}

bool empty_def(Deque_MyClass * deq_ptr)
{
	// returns true is empty
	if (deq_ptr -> length) return false;
	else return true;
}

void push_back_def(Deque_MyClass * deq_ptr, MyClass obj)
{
      deq_ptr -> data[deq_ptr -> back_index] = obj;
      deq_ptr -> back_index++;
      deq_ptr -> length++;
}

void push_front_def(Deque_MyClass * deq_ptr, MyClass obj)
{
      deq_ptr -> data[deq_ptr -> front_index] = obj;
      deq_ptr -> front_index--;
      deq_ptr -> length++;
}

MyClass & back_def(Deque_MyClass * deq_ptr)
{
      return deq_ptr -> data[deq_ptr -> back_index - 1];
}

MyClass & front_def(Deque_MyClass * deq_ptr)
{
      return deq_ptr -> data[deq_ptr -> front_index + 1];
}

void pop_back_def(Deque_MyClass * deq_ptr)
{
	// Should we remove the MyClass object from the deq here ?
      deq_ptr -> back_index--;
      deq_ptr -> length--;
}

void pop_front_def(Deque_MyClass * deq_ptr)
{
	// Should we remove the MyClass object from the deq here ?
      deq_ptr -> front_index++;
      deq_ptr -> length--;
}

void print_def(Deque_MyClass * deq_ptr)
{
	printf("length: %lu, front index: %lu, back index: %lu\n", deq_ptr -> length, deq_ptr -> front_index, deq_ptr -> back_index);
	for (size_t i = deq_ptr -> front_index + 1; i < deq_ptr -> length + deq_ptr -> front_index + 1; i++) 
	{
		printf("(%s %d) ", deq_ptr -> data[i].name, deq_ptr -> data[i].id);
	}
	printf("\n");
}

Deque_MyClass_Iterator begin_def(Deque_MyClass * deq_ptr)
{
	(deq_ptr -> begin_iter).iter = &(deq_ptr -> data[deq_ptr -> front_index + 1]);
      return deq_ptr -> begin_iter;
}

Deque_MyClass_Iterator end_def(Deque_MyClass * deq_ptr)
{
	(deq_ptr -> begin_iter).iter = &(deq_ptr -> data[deq_ptr -> back_index]);
      return deq_ptr -> begin_iter;
}

MyClass & at_def(Deque_MyClass * deq_ptr, size_t index)
{
	return deq_ptr -> data[deq_ptr -> front_index + 1 + index];
}

void clear_def(Deque_MyClass * deq_ptr)
{
	// Should I remove the elements here ? if so what should be the behavior ?
	deq_ptr -> length = 0;
	deq_ptr -> back_index = deq_ptr -> capacity / 2;
	deq_ptr -> front_index = deq_ptr -> back_index - 1;
}

void dtor_def(Deque_MyClass * deq_ptr)
{
	free(deq_ptr -> data);
}

void 
Deque_MyClass_ctor(Deque_MyClass * deq_ptr, bool (* comparator) (const MyClass &, const MyClass &))
{
      // That will call malloc in test.cpp
      deq_ptr -> capacity = 16;
	deq_ptr -> data = (MyClass *) malloc(deq_ptr -> capacity * sizeof(MyClass));
      
	deq_ptr -> length = 0;
	deq_ptr -> back_index = deq_ptr -> capacity / 2;
	deq_ptr -> front_index = deq_ptr -> back_index - 1;
	strcpy(deq_ptr -> type_name, "Deque_MyClass");

	deq_ptr -> size = size_def;
	deq_ptr -> empty = empty_def;

	deq_ptr -> push_back = push_back_def;
	deq_ptr -> push_front = push_front_def;

      deq_ptr -> back = back_def;
      deq_ptr -> front = front_def;

      deq_ptr -> pop_back = pop_back_def;
      deq_ptr -> pop_front = pop_front_def;

      deq_ptr -> begin = begin_def;
      deq_ptr -> end = end_def;

      (deq_ptr -> begin_iter).inc = inc_def;
      (deq_ptr -> begin_iter).dec = dec_def;
      (deq_ptr -> begin_iter).deref = deref_def;

      deq_ptr -> print = print_def;
      deq_ptr -> at = at_def;

      deq_ptr -> clear = clear_def;
      deq_ptr -> dtor = dtor_def;
}

bool
Deque_MyClass_equal(Deque_MyClass deq1, Deque_MyClass deq2)
{
      // Returns true if deqs are same length and all elements are equal
      if (deq1.length != deq2.length) return false;

      //Deque_MyClass_Iterator it_1 = deq1.begin(&deq1);
      //Deque_MyClass_Iterator it_2 = deq2.begin(&deq2);

      for (size_t i = 0; i < deq1.length; i++) 
      {
            MyClass a = deq1.data[i + deq1.front_index + 1];
            MyClass b = deq2.data[i + deq2.front_index + 1];
            
            //if (memcmp(&a, &b, sizeof(MyClass)) != 0) return false;

            //printf("%d, %s - %d, %s\n", it_1.iter -> id, it_1.iter -> name, it_2.iter -> id, it_2.iter -> name);

            //if (memcmp(it_1.iter, it_2.iter, sizeof(MyClass)) != 0) return false;

            //if (!Deque_MyClass_Iterator_equal(it_1, it_2)) return false;

            //it_1.inc(&it_1);
            //it_2.inc(&it_2);

            if (deq1.comparator(&a, &b)) return false;
            if (deq1.comparator(&a, &b)) return false;
      }

      return true;
}

int main() 
{
	Deque_MyClass deq;
	Deque_MyClass_ctor(&deq, MyClass_less_by_id);

	assert(deq.size(&deq) == 0);
    	assert(typeid(std::size_t) == typeid(decltype(deq.size(&deq))));
    	assert(deq.empty(&deq));
    	assert(sizeof deq.type_name == 14);

      MyClass oo_1;
      strcpy(oo_1.name, "Joe");
      oo_1.id = 1;

      MyClass oo_2;
      strcpy(oo_2.name, "Mary");
      oo_2.id = 2;

      MyClass oo_3;
      strcpy(oo_3.name, "Tom");
      oo_3.id = 3;

      MyClass oo_4;
      strcpy(oo_4.name, "Mike");
      oo_4.id = 0;

      MyClass oo_5;
      strcpy(oo_5.name, "Maryy");
      oo_5.id = -1;

    	deq.push_back(&deq, oo_1);
      deq.push_back(&deq, oo_2);
      deq.push_back(&deq, oo_3);
      deq.push_front(&deq, oo_4);
      deq.push_front(&deq, oo_5);

      //deq.push_back(&deq, MyClass{1, "Joe"});
      //deq.push_back(&deq, MyClass{2, "Mary"});
      //deq.push_back(&deq, MyClass{3, "Tom"});
      //deq.push_front(&deq, MyClass{0, "Mike"});
      //deq.push_front(&deq, MyClass{-1, "Mary"});

      assert(deq.front(&deq).id == -1);
      assert(deq.back(&deq).id == 3);

      deq.pop_front(&deq);
      deq.pop_back(&deq);

      assert(deq.front(&deq).id == 0);
      assert(deq.back(&deq).id == 2);
      assert(deq.size(&deq) == 3);

      for (Deque_MyClass_Iterator it = deq.begin(&deq); !Deque_MyClass_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) 
      {
      	//MyClass_print(&it.deref(&it));
      }

      for (Deque_MyClass_Iterator it1 = deq.begin(&deq); !Deque_MyClass_Iterator_equal(it1, deq.end(&deq)); it1.inc(&it1)) 
      {
      	//MyClass_print(&it1.deref(&it1));
            
            for (Deque_MyClass_Iterator it2 = deq.begin(&deq); !Deque_MyClass_Iterator_equal(it2, deq.end(&deq)); it2.inc(&it2)) 
            {
            	//MyClass_print(&it2.deref(&it2));

            	for (Deque_MyClass_Iterator it3 = deq.begin(&deq); !Deque_MyClass_Iterator_equal(it3, deq.end(&deq)); 
            		it3.inc(&it3))   
            	{
                  	//MyClass_print(&it3.deref(&it3));
            	}
            }
      }

      Deque_MyClass_Iterator it_4 = deq.end(&deq);
      it_4.dec(&it_4);
      assert(it_4.deref(&it_4).id == 2);

      //for (size_t i = 0; i < 3; i++) MyClass_print(&deq.at(&deq, i));

      assert(deq.at(&deq, 0).id == 0);
      deq.at(&deq, 0).id = 100;
      assert(deq.at(&deq, 0).id == 100);

      assert(deq.front(&deq).id == 100);
      deq.front(&deq).id = 0;
      assert(deq.front(&deq).id == 0);
      assert(deq.at(&deq, 0).id == 0);

      Deque_MyClass_Iterator it = deq.end(&deq);
      it.dec(&it);
      assert(it.deref(&it).id == 2);
      it.deref(&it).id = 200;
      assert(it.deref(&it).id == 200);

      assert(deq.back(&deq).id == 200);
      deq.back(&deq).id = 2;
      assert(deq.back(&deq).id == 2);
      assert(it.deref(&it).id == 2);

      //deq.print(&deq);
      deq.clear(&deq);
      deq.dtor(&deq);

      Deque_MyClass deq1, deq2;
      Deque_MyClass_ctor(&deq1, MyClass_less_by_id);
      Deque_MyClass_ctor(&deq2, MyClass_less_by_id);

      MyClass o_1;
      strcpy(o_1.name, "Joe");
      o_1.id = 1;
      deq1.push_back(&deq1, o_1);

      MyClass o_2;
      strcpy(o_2.name, "Jane");
      o_2.id = 2;
      deq1.push_back(&deq1, o_2);

      MyClass o_3;
      strcpy(o_3.name, "Mary");
      o_3.id = 3;
      deq1.push_back(&deq1, o_3);

      MyClass o_4;
      strcpy(o_4.name, "Joe");
      o_4.id = 1;
      deq2.push_back(&deq2, o_4);

      MyClass o_5;
      strcpy(o_5.name, "Jane");
      o_5.id = 2;
      deq2.push_back(&deq2, o_5);

      MyClass o_6;
      strcpy(o_6.name, "Mary");
      o_6.id = 3;
      deq2.push_back(&deq2, o_6);

      assert(Deque_MyClass_equal(deq1, deq2));

      deq1.pop_back(&deq1);
      assert(!Deque_MyClass_equal(deq1, deq2));

      MyClass o_7;
      strcpy(o_7.name, "Mary");
      o_7.id = 4;
      deq1.push_back(&deq1, o_7);
      assert(!Deque_MyClass_equal(deq1, deq2));

      deq1.dtor(&deq1);
      deq2.dtor(&deq2);
}
