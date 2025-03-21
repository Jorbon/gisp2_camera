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



// copied from pwm.c
void xbar_connect(unsigned int input, unsigned int output)
{
	if (input >= 88) return;
	if (output >= 132) return;

	volatile uint16_t *xbar = &XBARA1_SEL0 + (output / 2);
	uint16_t val = *xbar;
	if (!(output & 1)) {
		val = (val & 0xFF00) | input;
	} else {
		val = (val & 0x00FF) | (input << 8);
	}
	*xbar = val;
}



#endif