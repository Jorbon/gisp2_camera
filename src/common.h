#ifndef COMMON_H
#define COMMON_H

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#define let auto


struct Unit {};
#define UNIT Unit {}

enum ResultType {
	Ok,
	Err,
};

template <typename T, typename E = const char*>
union ResultValue {
	T ok_value;
	E err_value;
};

template <typename T, typename E = const char*>
struct Result {
	ResultValue<T, E> value;
	ResultType type;
	
	static Result<T, E> ok(T ok_value) {
		return Result {
			.value = ResultValue<T, E> { .ok_value = ok_value },
			.type = ResultType::Ok,
		};
	}
	static Result<T, E> err(E err_value) {
		return Result {
			.value = ResultValue<T, E> { .err_value = err_value },
			.type = ResultType::Err,
		};
	}
	
	T unwrap() const {
		if (this->type != ResultType::Ok) {
			fprintf(stderr, "Tried to unwrap err value: %s\n", this->value.err_value);
			exit(1);
		}
		
		return this->value.ok_value;
	}
};



#endif