#ifndef DEQUE_HPP
#define DEQUE_HPP

#define Deque_DEFINE(type)													\
																	\
	struct Deque_##type##_Iterator 	      									\
	{																\
      	type * iter;                                                                              \
                                                                                                      \
            int index;                                                                                \
            int deq_capacity;                                                                         \
                                                                                                      \
            type * first_data;                                                                        \
            type * last_data;													\
      																\
      	void (* inc) (Deque_##type##_Iterator *);									\
      	void (* dec) (Deque_##type##_Iterator *);									\
      	type & (* deref) (Deque_##type##_Iterator *);								\
	};																\
																	\
	type & deref_def(Deque_##type##_Iterator * deq_iter_ptr)							\
	{																\
		return *(deq_iter_ptr -> iter);										\
	}																\
																	\
	void inc_def(Deque_##type##_Iterator * deq_iter_ptr)								\
	{				                                                                        \
            deq_iter_ptr -> index++;                                                                  \
                                                                                                      \
            if (deq_iter_ptr -> index == deq_iter_ptr -> deq_capacity)                                \
            {                                                                                         \
                  deq_iter_ptr -> index = 0;                                                          \
                  deq_iter_ptr -> iter = deq_iter_ptr -> first_data;                                  \
            }                                                                                         \
            else if (deq_iter_ptr -> index > deq_iter_ptr -> deq_capacity) assert(false);             \
            else deq_iter_ptr -> iter++;											\
	}																\
																	\
	void dec_def(Deque_##type##_Iterator * deq_iter_ptr)								\
	{					                                                                  \
            deq_iter_ptr -> index--;                                                                  \
                                                                                                      \
            if (deq_iter_ptr -> index == -1)                                                          \
            {                                                                                         \
                  deq_iter_ptr -> index = deq_iter_ptr -> deq_capacity;                               \
                  deq_iter_ptr -> iter = deq_iter_ptr -> last_data;                                   \
            }                                                                                         \
            else if (deq_iter_ptr -> index < -1) assert(false);                                       \
            else deq_iter_ptr -> iter--;											\
	}																\
																	\
	bool																\
	Deque_##type##_Iterator_equal(Deque_##type##_Iterator it, Deque_##type##_Iterator end)		\
	{																\
		/* Returns True if the iterators point to the same MyClass object */				\
	      if (it.iter == end.iter) return true;									\
	      else return false; 												\
	}																\
																	\
	struct Deque_##type							\
	{										\
		type * data;							\
		int capacity;							\
    		size_t length;	                                    \
            					                   	\
    		int front_index;						      \
    		int back_index;						      \
    		char type_name[sizeof("Deque_"#type)];	  		\
    											\
    		size_t (* size) (Deque_##type * deq_ptr);  		\
    		bool (* empty) (Deque_##type * deq_ptr);			\
    											\
    		void (* push_back) (Deque_##type *, type);		\
    		void (* push_front) (Deque_##type *, type);		\
    											\
      	type & (* back) (Deque_##type *);				\
      	type & (* front) (Deque_##type *);				\
      										\
      	void (* pop_back) (Deque_##type *);				\
      	void (* pop_front) (Deque_##type *);			\
      										\
      	Deque_##type##_Iterator begin_iter;				\
    											\
    		Deque_##type##_Iterator (* begin) (Deque_##type *);	\
      	Deque_##type##_Iterator (* end) (Deque_##type *);	\
      										\
      	type & (* at) (Deque_##type * deq_ptr, size_t index);	\
            void (* clear) (Deque_##type * deq_ptr);              \
            void (* dtor) (Deque_##type * deq_ptr);               \
                                                                                                \
            void (* sort) (Deque_##type *, Deque_##type##_Iterator, Deque_##type##_Iterator);   \
            bool (* comparator) (const type &, const type &);                                   \
	};										\
											\
	size_t 									\
	size_def(Deque_##type * deq_ptr)					\
	{										\
		return deq_ptr -> length;					\
	}										\
											\
	bool										\
	empty_def(Deque_##type * deq_ptr)					\
	{										\
		if (deq_ptr -> length) return false;			\
		else return true;							\
	}										\
                                                                                                            \
      bool resize(Deque_##type * deq_ptr)                                                                   \
      {                                                                                                     \
            if ((int) deq_ptr -> length == deq_ptr -> capacity - 2)                                         \
            {                                                                                               \
                  deq_ptr -> capacity *= 2;                                                                 \
                  deq_ptr -> data = (type *) realloc(deq_ptr -> data, deq_ptr -> capacity * sizeof(type));  \
                  return true;                                                                              \
            }                                                                                               \
            else if ((int) deq_ptr -> length >= deq_ptr -> capacity - 1) assert(false);                     \
                                                                                                            \
            return false;                                                                                   \
      }                                                                                                     \
                                                                                                            \
      void rearrange(Deque_##type * deq_ptr)                                                                \
      {                                                                                                     \
            int old_capacity = deq_ptr -> capacity / 2;                                                     \
                                                                                                            \
            if (deq_ptr -> back_index > deq_ptr -> front_index) return;                                     \
                                                                                                            \
            int num_data_to_move = old_capacity - deq_ptr -> front_index - 1;                               \
            int dist_to_beg = old_capacity - num_data_to_move;                                              \
                                                                                                            \
            for (int i = 0; i < num_data_to_move; i++)                                                      \
            {                                                                                               \
                  deq_ptr -> data[dist_to_beg + old_capacity + i] = deq_ptr -> data[dist_to_beg + i];       \
            }                                                                                               \
                                                                                                            \
            deq_ptr -> front_index += old_capacity;                                                         \
      }                                                                                                     \
											                                          \
	void push_back_def(Deque_##type * deq_ptr, type obj)		                                          \
	{										                                          \
            deq_ptr -> length++;                                                                            \
            deq_ptr -> data[deq_ptr -> back_index] = obj;                                                   \
                                                                                                            \
            deq_ptr -> back_index = (deq_ptr -> back_index + 1) % deq_ptr -> capacity;                      \
                                                                                                            \
            bool resized = resize(deq_ptr);                                                                 \
            if (resized) rearrange(deq_ptr);                                                                \
	}										                                          \
											                                          \
	void push_front_def(Deque_##type * deq_ptr, type obj)		                                          \
	{										                                          \
	      deq_ptr -> length++;                                                                            \
            deq_ptr -> data[deq_ptr -> front_index] = obj;                                                  \
                                                                                                            \
            deq_ptr -> front_index--;                                                                       \
            while (deq_ptr -> front_index < 0) deq_ptr -> front_index += deq_ptr -> capacity;               \
                                                                                                            \
            bool resized = resize(deq_ptr);                                                                 \
            if (resized) rearrange(deq_ptr);                                                                \
	}										\
											\
	type & back_def(Deque_##type * deq_ptr)				\
	{										\
            int target = deq_ptr -> back_index - 1;               \
            while (target < 0) target += deq_ptr -> capacity;     \
                                                                  \
            return deq_ptr -> data[target];	                  \
	}										\
											\
	type & front_def(Deque_##type * deq_ptr)				\
	{										\
            return deq_ptr -> data[(deq_ptr -> front_index + 1) % deq_ptr -> capacity];	\
	}										\
											\
	void pop_back_def(Deque_##type * deq_ptr)				\
	{										\
      	deq_ptr -> back_index--;                              \
                                                                  \
            while (deq_ptr -> back_index < 0) deq_ptr -> back_index += deq_ptr -> capacity;	\
      	deq_ptr -> length--;						                              \
	}										                              \
											                              \
	void pop_front_def(Deque_##type * deq_ptr)			                              \
	{										                              \
	      deq_ptr -> front_index = (deq_ptr -> front_index + 1) % deq_ptr -> capacity;		\
	      deq_ptr -> length--;						                              \
	}										                              \
																\
	Deque_##type##_Iterator begin_def(Deque_##type * deq_ptr)						\
	{															\
		int target = (deq_ptr -> front_index + 1) % deq_ptr -> capacity;                    \
                                                                                                \
            (deq_ptr -> begin_iter).iter = &(deq_ptr -> data[target]);                          \
            (deq_ptr -> begin_iter).index = target;                                             \
                                                                                                \
            (deq_ptr -> begin_iter).first_data = &(deq_ptr -> data[0]);                         \
            (deq_ptr -> begin_iter).last_data = &(deq_ptr -> data[deq_ptr -> capacity - 1]);    \
                                                                                                \
            (deq_ptr -> begin_iter).deq_capacity = deq_ptr -> capacity;                         \
                                                                                                \
            return deq_ptr -> begin_iter;									      \
	}															\
																\
	Deque_##type##_Iterator end_def(Deque_##type * deq_ptr)						\
	{															\
            (deq_ptr -> begin_iter).iter = &(deq_ptr -> data[deq_ptr -> back_index]);           \
            (deq_ptr -> begin_iter).index = deq_ptr -> back_index;                              \
                                                                                                \
            (deq_ptr -> begin_iter).first_data = &(deq_ptr -> data[0]);                         \
            (deq_ptr -> begin_iter).last_data = &(deq_ptr -> data[deq_ptr -> capacity - 1]);    \
                                                                                                \
            (deq_ptr -> begin_iter).deq_capacity = deq_ptr -> capacity;                         \
                                                                                                \
            return deq_ptr -> begin_iter;                                                       \
	}															\
																\
	type & at_def(Deque_##type * deq_ptr, size_t index)							\
	{															\
		int target = deq_ptr -> front_index + 1 + index;                                    \
            while (target >= (int) deq_ptr -> capacity) target -= deq_ptr -> capacity;          \
                                                                                                \
            return deq_ptr -> data[target];				                              \
	}															\
                                                                                                \
      void clear_def(Deque_##type * deq_ptr)                                                    \
      {                                                                                         \
            /* Should I remove the elements here ? if so what should be the behavior */         \
            deq_ptr -> length = 0;                                                              \
            deq_ptr -> back_index = deq_ptr -> capacity / 2;                                    \
            deq_ptr -> front_index = deq_ptr -> back_index - 1;                                 \
      }                                                                                         \
                                                                                                \
      void dtor_def(Deque_##type * deq_ptr)                                                     \
      {                                                                                         \
            free(deq_ptr -> data);                                                              \
      }                                                                                         \
																\
      void sort_swap(type * i, type * j)                                                        \
      {                                                                                         \
            type tmp = *i;                                                                      \
            *i = *j;                                                                            \
            *j = tmp;                                                                           \
      }                                                                                         \
                                                                                                \
      Deque_##type##_Iterator sort_partition(Deque_##type * deq, Deque_##type##_Iterator low, Deque_##type##_Iterator high)   \
      {                                                                                                                       \
            type * pivot = high.iter;                                                                                         \
                                                                                                                              \
            Deque_##type##_Iterator i;                                                                                        \
            i.iter = low.iter - 1;                                                                                            \
                                                                                                                              \
            for (type * j = low.iter; j <= high.iter - 1; j++)                                                                \
            {                                                                                                                 \
                  /* returns True if first obj is smaller */                                                                  \
                  if (deq -> comparator(*j, *pivot))                                                                          \
                  {                                                                                                           \
                        i.iter++;                                                                                             \
                        sort_swap(i.iter, j);                                                                                 \
                  }                                                                                                           \
            }                                                                                                                 \
                                                                                                                              \
            i.iter++;                                                                                                         \
            sort_swap(i.iter, high.iter);                                                                                     \
                                                                                                                              \
            return i;                                                                                                         \
      }                                                                                                                       \
                                                                                                                              \
      void sort_orig(Deque_##type * deq_ptr, Deque_##type##_Iterator beg, Deque_##type##_Iterator end)                        \
      {                                                                                                                       \
            if (beg.iter < end.iter)                                                                                          \
            {                                                                                                                 \
                  Deque_##type##_Iterator pi = sort_partition(deq_ptr, beg, end);                                             \
                                                                                                                              \
                  pi.iter--;                                                                                                  \
                  sort_orig(deq_ptr, beg, pi);                                                                                \
                                                                                                                              \
                  pi.iter += 2;                                                                                               \
                  sort_orig(deq_ptr, pi, end);                                                                                \
            }                                                                                                                 \
      }                                                                                                                       \
                                                                                                                              \
      void iter_dec(Deque_##type##_Iterator * deq_iter_ptr)                                                                   \
      {                                                                                                                       \
            deq_iter_ptr -> index--;                                                                                          \
                                                                                                                              \
            if (deq_iter_ptr -> index == -1)                                                                                  \
            {                                                                                                                 \
                  deq_iter_ptr -> index = deq_iter_ptr -> deq_capacity;                                                       \
                  deq_iter_ptr -> iter = deq_iter_ptr -> last_data;                                                           \
            }                                                                                                                 \
            else if (deq_iter_ptr -> index < -1) assert(false);                                                               \
            else deq_iter_ptr -> iter--;                                                                                      \
      }                                                                                                                       \
                                                                                                                              \
      void sort_def (Deque_##type * deq_ptr, Deque_##type##_Iterator beg, Deque_##type##_Iterator end)                        \
      {                                                                                                                       \
            iter_dec(&end); /* Because end iterator is end + 1 (back_index) */                                                \
                                                                                                                              \
            if (end.index > beg.index) /* data is ordered */                                                                  \
            {                                                                                                                 \
                  if (beg.iter < end.iter)                                                                                    \
                  {                                                                                                           \
                        Deque_##type##_Iterator pi = sort_partition(deq_ptr, beg, end);                                       \
                                                                                                                              \
                        pi.iter--;                                                                                            \
                        sort_orig(deq_ptr, beg, pi);                                                                          \
                                                                                                                              \
                        pi.iter += 2;                                                                                         \
                        sort_orig(deq_ptr, pi, end);                                                                          \
                  }                                                                                                           \
            }                                                                                                                 \
            else /* data is wrapped around */                                                                                 \
            {                                                                                                                 \
                  type * data_tmp = (type *) malloc(deq_ptr -> capacity * sizeof(type));                                      \
                                                                                                                              \
                  for (size_t i = 0; i < deq_ptr -> length; i++)                                                              \
                  {                                                                                                           \
                        data_tmp[i] = deq_ptr -> data[(deq_ptr -> front_index + 1 + i) % deq_ptr -> capacity];                \
                  }                                                                                                           \
                                                                                                                              \
                  for (size_t i = 0; i < deq_ptr -> length; i++) deq_ptr -> data[i + 1] = data_tmp[i];                        \
                                                                                                                              \
                  deq_ptr -> front_index = 0;                                                                                 \
                  deq_ptr -> back_index = deq_ptr -> length + 1;                                                              \
                                                                                                                              \
                  Deque_##type##_Iterator beg_new = deq_ptr -> begin(deq_ptr);                                                \
                  Deque_##type##_Iterator end_new = deq_ptr -> end(deq_ptr);                                                  \
                  iter_dec(&end_new);                                                                                         \
                                                                                                                              \
                  if (beg_new.iter < end_new.iter)                                                                            \
                  {                                                                                                           \
                        Deque_##type##_Iterator pi = sort_partition(deq_ptr, beg_new, end_new);                               \
                                                                                                                              \
                        pi.iter--;                                                                                            \
                        sort_orig(deq_ptr, beg_new, pi);                                                                      \
                                                                                                                              \
                        pi.iter += 2;                                                                                         \
                        sort_orig(deq_ptr, pi, end_new);                                                                      \
                  }                                                                                                           \
                                                                                                                              \
                  free(data_tmp);                                                                                             \
            }                                                                                                                 \
      }                                                                                                                       \
                                                                                                                              \
	void 															      \
	Deque_##type##_ctor(Deque_##type * deq_ptr, bool (* cmp) (const type &, const type &))	      \
	{																\
		/* That will call malloc in test.cpp */									\
		deq_ptr -> capacity = 10;       									      \
		deq_ptr -> data = (type *) malloc(deq_ptr -> capacity * sizeof(type));				\
																	\
		deq_ptr -> length = 0;												\
		deq_ptr -> back_index = deq_ptr -> capacity / 2;							\
		deq_ptr -> front_index = deq_ptr -> back_index - 1;							\
																	\
		strcpy(deq_ptr -> type_name, "Deque_"#type);								\
		deq_ptr -> size = size_def;											\
		deq_ptr -> empty = empty_def;											\
																	\
		deq_ptr -> push_back = push_back_def;									\
		deq_ptr -> push_front = push_front_def;									\
																	\
      	deq_ptr -> back = back_def;											\
      	deq_ptr -> front = front_def;											\
      																\
      	deq_ptr -> pop_back = pop_back_def;										\
      	deq_ptr -> pop_front = pop_front_def;									\
      																\
      	deq_ptr -> begin = begin_def;											\
      	deq_ptr -> end = end_def;											\
      																\
      	(deq_ptr -> begin_iter).inc = inc_def;									\
      	(deq_ptr -> begin_iter).dec = dec_def;									\
      	(deq_ptr -> begin_iter).deref = deref_def;                                                \
            (deq_ptr -> begin_iter).deq_capacity = deq_ptr -> capacity;						\
      																\
      	deq_ptr -> at = at_def;		    		                                                \
            deq_ptr -> clear = clear_def;                                                             \
            deq_ptr -> dtor = dtor_def;					                      			\
                                                                                                      \
            deq_ptr -> sort = sort_def;                                                               \
            deq_ptr -> comparator = cmp;                                                              \
	}							                                                      \
                                                                                                      \
      bool                                                                                            \
      Deque_##type##_equal(Deque_##type deq1, Deque_##type deq2)                                      \
      {                                                                                               \
            /* Returns true if deqs are same length and all elements are equal */                     \
            if (deq1.length != deq2.length) return false;                                             \
                                                                                                      \
            for (int i = 0; i < (int) deq1.length; i++)                                               \
            {                                                                                         \
                  type a = deq1.at(&deq1, i);                                                         \
                  type b = deq2.at(&deq2, i);                                                         \
                                                                                                      \
                  if (deq1.comparator(a, b)) return false;                                            \
                  if (deq1.comparator(b, a)) return false;                                            \
            }                                                                                         \
                                                                                                      \
            return true;                                                                              \
      }       

#endif                                                                                        
