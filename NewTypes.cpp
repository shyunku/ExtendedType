#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

#define MAXS(a,b)					((a>b)?a:b)
#define MINS(a,b)					((a<b)?a:b)
#define MAX_PRECISION				32

using namespace std;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

namespace newtypes {
	/*

	<---32bit 컴퓨터 및 32bit compile 환경 기반--->

	1. Strint ( triple )
	자료형 :  Extended Double
	범위 : -(10^4294967294 - 1) ~ (10^4294967294 - 1)
	자리수 표현 : 소수점 포함 최대 4,294,967,294 자리
	크기 : 최소 1 Byte ~ 최대 4.09 GB
	연산 : addition, subtraction, multiplication, division, comparison
	연산 속도 : 200,000 자리 수 덧셈 기준 5658ms 소요

	2. BigInteger64 ( bint64 )
	자료형 :  Integer
	범위 : -(10^19327352832 - 1) ~ (10^19327352832 - 1)
	자리수 표현 : 최대 19,327,352,832 자리
	크기 : 최소 8 Byte ~ 최대 8.59 GB
	연산 : addition, subtraction, comparison
	연산 속도 : 200,000 자리 수 덧셈 기준 22ms 소요

	3. BigInteger32 ( bint32 )
	자료형 :  Integer
	범위 : -(10^8589934592 - 1) ~ (10^8589934592 - 1)
	자리수 표현 : 최대 8,589,934,592 자리
	크기 : 최소 4 Byte ~ 최대 4.29 GB
	연산 : addition, subtraction, multiplication, division, comparison
	연산 속도 : 200,000 자리 수 덧셈 기준 47ms 소요 예상

	<---64bit 컴퓨터 및 64bit compile 환경 기반--->

	1. Strint ( triple )
	자료형 :  Extended Double
	범위 : -(10^18446744073709551614 - 1) ~ (10^18446744073709551614 - 1)
	자리수 표현 : 소수점 포함 최대 18,446,744,073,709,551,614 자리
	크기 : 최소 1 Byte ~ 최대 8.21 GB
	연산 : addition, subtraction, multiplication, division, comparison
	연산 속도 : 32비트와 동일

	2. BigInteger64 ( bint64 )
	자료형 :  Integer
	범위 : -(10^83010348331692982254 - 1) ~ (10^83010348331692982254 - 1)
	자리수 표현 : 최대 83,010,348,331,692,982,254 자리
	크기 : 최소 8 Byte ~ 최대 17.02 GB
	연산 : addition, subtraction, comparison
	연산 속도 : 32비트와 동일

	3. BigInteger32 ( bint32 )
	자료형 :  Integer
	범위 : -(10^36893488147419103224 - 1) ~ (10^36893488147419103224 - 1)
	자리수 표현 : 최대 36,893,488,147,419,103,224 자리
	크기 : 최소 4 Byte ~ 최대 8.59 GB
	연산 : addition, subtraction, multiplication, division, comparison
	연산 속도 : 32비트와 동일 예상

	*/
	const u64 BINT_MAX = 999999999999999999;
	const u64 BINT_MAX_DIGIT = 18;
	typedef class strint {
	public:
		string all_digit;
		int point;
		bool sign;
		strint() {
			this->sign = false;
			this->all_digit = "";
			this->point = 0;
		}
		strint(string str) {
			this->sign = false;
			this->all_digit = "";
			this->point = 0;
			this->setstrint_string(str);
		}
		bool operator>(const strint& t1) {
			//sign
			if (t1.sign && !this->sign) return true;
			if (!t1.sign && this->sign) return false;
			if (t1.sign&&this->sign) return make_oppsign(*this) > make_oppsign(t1);
			//point
			if (this->point > t1.point) return true;
			if (t1.point > this->point)	return false;
			for (int i = 0; i < MINS(t1.all_digit.length(), this->all_digit.length()); i++)
				if (this->all_digit[i] > t1.all_digit[i])
					return true;
				else if (this->all_digit[i] < t1.all_digit[i])
					return false;
			if (this->all_digit.length() > t1.all_digit.length())
				return true;
			return false;
		}
		bool operator>=(const strint& t1) {
			//sign
			if (t1.sign && !this->sign) return true;
			if (!t1.sign && this->sign) return false;
			if (t1.sign&&this->sign) return make_oppsign(*this) > make_oppsign(t1);
			//point
			if (this->point > t1.point) return true;
			if (t1.point > this->point)	return false;
			for (int i = 0; i < MINS(t1.all_digit.length(), this->all_digit.length()); i++)
				if (this->all_digit[i] > t1.all_digit[i])
					return true;
				else if (this->all_digit[i] < t1.all_digit[i])
					return false;
			if (this->all_digit.length() > t1.all_digit.length())
				return true;
			if (t1.all_digit.length() > this->all_digit.length())
				return false;
			return true;
		}
		bool operator<(const strint& t1) {
			//sign
			if (t1.sign && !this->sign) return false;
			if (!t1.sign && this->sign) return true;
			if (t1.sign&&this->sign) return make_oppsign(*this) < make_oppsign(t1);
			//point
			if (this->point > t1.point) return false;
			if (t1.point > this->point)	return true;
			for (int i = 0; i < MINS(t1.all_digit.length(), this->all_digit.length()); i++)
				if (this->all_digit[i] < t1.all_digit[i])
					return true;
				else if (this->all_digit[i] > t1.all_digit[i])
					return false;
			if (this->all_digit.length() < t1.all_digit.length())
				return true;
			return false;
		}
		bool operator<=(const strint& t1) {
			//sign
			if (t1.sign && !this->sign) return false;
			if (!t1.sign && this->sign) return true;
			if (t1.sign&&this->sign) return make_oppsign(*this) < make_oppsign(t1);
			//point
			if (this->point > t1.point) return false;
			if (t1.point > this->point)	return true;
			for (int i = 0; i < MINS(t1.all_digit.length(), this->all_digit.length()); i++)
				if (this->all_digit[i] < t1.all_digit[i])
					return true;
				else if (this->all_digit[i] > t1.all_digit[i])
					return false;
			if (this->all_digit.length() < t1.all_digit.length())
				return true;
			if (this->all_digit.length() < t1.all_digit.length())
				return false;
			return true;
		}
		bool operator==(const strint& t1) {
			//sign
			if (t1.sign && !this->sign) return false;
			if (!t1.sign && this->sign) return false;
			if (t1.sign&&this->sign) return make_oppsign(*this) < make_oppsign(t1);
			//point
			if (this->point > t1.point) return false;
			if (t1.point > this->point)	return false;
			for (int i = 0; i < MINS(t1.all_digit.length(), this->all_digit.length()); i++)
				if (this->all_digit[i] < t1.all_digit[i])
					return false;
				else if (this->all_digit[i] > t1.all_digit[i])
					return false;
			if (this->all_digit.length() < t1.all_digit.length())
				return false;
			return true;
		}
		strint operator+(const strint& t1) {
			strint returns;
			//First Exception - Zero I
			if (t1.all_digit == "") {
				returns = *this;
				return returns;
			}
			if (this->all_digit == "") {
				returns.sign = t1.sign;
				returns.all_digit = t1.all_digit;
				returns.point = t1.point;
				return returns;
			}
			//Second Exception - 
			if (this->sign ^ t1.sign)
			{
				//add operator reference
				if (t1.sign) {
					//a-(-b)
					return *this - make_oppsign(t1);
				}
				else {
					//b-a
					return make_oppsign(make_oppsign(t1)) - make_oppsign(*this);
				}
			}
			else
			{
				if (t1.sign) {
					//-a-b = -(a+b)
					return make_oppsign(make_oppsign(*this) + make_oppsign(t1));
				}
			}

			int larg_len = MAXS(t1.all_digit.length(), this->all_digit.length());
			string temp = t1.all_digit;
			int temp_point = t1.point;
			string thisone_string = this->all_digit;
			int this_point = this->point;

			/* --- adjust format --- */

			//point adjusting
			if (this_point > temp_point) {
				for (int i = 0; this_point != temp_point; i++) {
					temp = '0' + temp;
					temp_point++;
				}
			}
			else {
				for (int i = 0; this_point != temp_point; i++) {
					thisone_string = '0' + thisone_string;
					this_point++;
				}
			}
			//length adjusting
			if (thisone_string.length() < temp.length()) {
				for (int i = 0; temp.length() != thisone_string.length(); i++) {
					thisone_string = thisone_string + '0';
				}
			}
			else {
				for (int i = 0; thisone_string.length() != temp.length(); i++) {
					temp = temp + '0';
				}
			}

			//real calculation
			int up_digit = 0, saver = 0;
			for (int i = temp.length() - 1; i >= 0; i--) {
				saver = thisone_string[i] + up_digit + temp[i] - 2 * '0';
				if (saver < 10) {
					up_digit = 0;
					returns.all_digit = (char)(saver + '0') + returns.all_digit;
				}
				else {
					up_digit = 1;
					returns.all_digit = (char)((saver - 10) + '0') + returns.all_digit;
				}
			}
			if (up_digit > 0) {
				returns.all_digit = (char)(up_digit + '0') + returns.all_digit;
				temp_point++;
			}
			//cut left zero part
			int leftzero = 0, rightzero = 0; string newline = "";
			for (int i = 0; returns.all_digit[i] == '0'; i++, leftzero++);
			//for (int i = returns.all_digit.length() - 1; returns.all_digit[i] == '0'; i--, rightzero++);
			for (int i = leftzero; i < returns.all_digit.length() - rightzero; i++) newline = newline + returns.all_digit[i];
			returns.all_digit = newline; returns.point = temp_point - leftzero;

			returns = clean_type(returns);

			return returns;
		}
		strint operator-(const strint& t1) {
			strint returns;
			//Initial Exception - Zero I
			if ((this->all_digit == t1.all_digit) && (this->point == t1.point) && (this->sign == t1.sign))
				return make_zero();
			//First Exception - Zero II
			if (t1.all_digit == "") {
				returns = *this;
				return returns;
			}
			if (this->all_digit == "") {
				returns.sign = !t1.sign;
				returns.all_digit = t1.all_digit;
				returns.point = t1.point;
				return returns;
			}
			//Second Exception - 
			if (this->sign ^ t1.sign)
			{
				//add operator reference
				if (t1.sign) {
					//a-(-b)
					return *this + make_oppsign(t1);
				}
				else {
					//-a-b
					return make_oppsign(*this) + t1;
				}
			}
			else
			{
				if (t1.sign) {
					//-a-(-b) = b-a
					switch (absolute_compare(t1)) {
					case 1:
						return make_oppsign(*this - make_oppsign(t1));
					case -1:
						return make_oppsign(t1) - make_oppsign(*this);
					case 0:
						return make_zero();
					}
				}
				else {
					//a-b
					switch (absolute_compare(t1)) {
					case 1:
						break;		//continue
					case -1:
						return make_oppsign(make_oppsign(make_oppsign(t1)) - *this);
					case 0:
						return make_zero();
					}
				}
			}
			int larg_len = MAXS(t1.all_digit.length(), this->all_digit.length());
			string temp = t1.all_digit;
			int temp_point = t1.point;
			string thisone_string = this->all_digit;
			int this_point = this->point;
			/* --- adjust format --- */

			//point adjusting
			for (int i = 0; this_point != temp_point; i++) {
				temp = '0' + temp;
				temp_point++;
			}
			//length adjusting
			if (thisone_string.length() < temp.length()) {
				for (int i = 0; temp.length() != thisone_string.length(); i++) {
					thisone_string = thisone_string + '0';
				}
			}
			else {
				for (int i = 0; thisone_string.length() != temp.length(); i++) {
					temp = temp + '0';
				}
			}
			//real calculation
			int up_digit = 0, saver = 0;
			for (int i = temp.length() - 1; i >= 0; i--) {
				saver = thisone_string[i] - up_digit - temp[i];
				if (saver >= 0) {
					up_digit = 0;
					returns.all_digit = (char)(saver + '0') + returns.all_digit;
				}
				else {
					up_digit = 1;
					returns.all_digit = (char)((saver + 10) + '0') + returns.all_digit;
				}
			}
			//cut left zero part
			int leftzero = 0, rightzero = 0; string newline = "";
			for (int i = 0; returns.all_digit[i] == '0'; i++, leftzero++);
			for (int i = leftzero; i < returns.all_digit.length(); i++) newline = newline + returns.all_digit[i];
			returns.all_digit = newline; returns.point = temp_point - leftzero;
			returns = clean_type(returns);

			return returns;
		}
		strint operator/(const strint& t1) {
			strint returns;
			strint data[2]; data[0] = *this; data[1] = t1;
			returns.sign = data[0].sign ^ data[1].sign;

			int left_prec = this->all_digit.length() - this->point;
			int right_prec = t1.all_digit.length() - t1.point;
			int diff_prec = left_prec - right_prec;						//will be divided in final

																		//point formatting
			data[0].point += left_prec;
			data[1].point += right_prec;

			//sign standardize
			data[0].sign = false;
			data[1].sign = false;

			int adjust_prec = data[1].all_digit.length() - 1 + MAX_PRECISION;
			data[0].point += adjust_prec;
			diff_prec += adjust_prec;

			//right zero merging
			for (int i = 0; i < data[1].all_digit.length() - 1 + adjust_prec; i++)
				data[0].all_digit = data[0].all_digit + '0';

			int MAXS_digit_flag = 0;
			for (; data[0] >= get_Powed_10(data[1], MAXS_digit_flag); MAXS_digit_flag++);
			--MAXS_digit_flag;

			for (int i = MAXS_digit_flag; i >= 0; i--) {
				int try_subtract_num = 0;
				strint saver = get_Powed_10(data[1], i);
				for (int j = 0; j < 9; j++) {
					if (data[0] < saver) break;
					data[0] = data[0] - saver;
					try_subtract_num++;
				}
				returns.all_digit = returns.all_digit + (char)(try_subtract_num + '0');
			}
			returns.point = 0;
			returns.point = returns.all_digit.length() - diff_prec;

			return returns;
		}
		strint operator%(const strint& t1) {
			strint& th1 = *this, th2 = t1;

		}
		int absolute_compare(const strint& t1) {
			if (this->point > t1.point) return 1;
			if (t1.point > this->point)return -1;
			for (int i = 0; i < MINS(t1.all_digit.length(), this->all_digit.length()); i++)
				if (this->all_digit[i] > t1.all_digit[i])
					return 1;
				else if (this->all_digit[i] < t1.all_digit[i])
					return -1;
			if (this->all_digit.length() > t1.all_digit.length())
				return 1;
			if (t1.all_digit.length() > this->all_digit.length())
				return -1;
			return 0;
		}
		strint make_zero() {
			strint tmp;
			return tmp;
		}
		strint make_oppsign(strint t) {
			strint tmp = t;
			tmp.sign = !tmp.sign;
			return tmp;
		}
		//must be modified
		template <typename T>
		void setstrint(T obj) {//set default floating point to 8
			this->sign = obj < 0;
			obj *= (T)(this->sign ? -1 : 1);
			this->point = 0;
			int sep1 = (int)obj;
			if (sep1 == obj) {
				for (; sep1 > 0; sep1 /= 10) {
					all_digit = all_digit + (char)((sep1 % 10) + '0');
					this->point++;
				}
				return;
			}
			double sep2 = (double)(obj - (int)obj);
			if (sep2 >= 1) return (void)(cout << "error" << endl);
			sep2 *= 10;
			for (int i = 0; i < 8; i++, sep2 *= 10)
				all_digit = all_digit + (char)((int)sep2 + '0');
			return;
		}
		void setstrint_string(string str) {
			if (str == "0") {
				*this = make_zero();
				return;
			}
			this->sign = str[0] == '-';
			if (this->sign)str.erase(str.begin());

			int dot_index = str.find(".");
			if (dot_index != -1) {
				str.erase(str.begin() + dot_index);
				//floating point handling
				if ((dot_index == 1) && (str[0] == '0')) {
					int MINSus_point = 0;
					for (int i = 1; i<str.length(); i++, MINSus_point++)
						if (str[i] != '0')break;
					this->point = -MINSus_point;
				}
				else {
					this->point = dot_index;
				}
			}
			else
				this->point = str.length();

			//cut left zero part
			int leftzero = 0; string newline = "";
			for (int i = 0; str[i] == '0'; i++, leftzero++);
			for (int i = leftzero; i < str.length(); i++) newline = newline + str[i];
			this->all_digit = newline;
		}
		strint get_Powed_10(strint t, int n) {
			strint tmp = t;
			tmp.point += n;
			return tmp;
		}
		//error
		strint clean_type(strint t) {
			strint returns = t;

			int rightzero = 0;
			for (int i = returns.all_digit.length() - 1; i >= returns.point; i--, rightzero++)
				if (returns.all_digit[i] != '0')break;
			returns.all_digit = returns.all_digit.substr(0, returns.all_digit.length() - rightzero);

			return returns;
		}
		//must be modified
		void print_with_precision(int precision) {
			if (this->sign)cout << "-";
			for (int i = 0; i < precision + this->point; i++) {
				if (i >= this->all_digit.length())
					cout << "0";
				else
					cout << this->all_digit[i];
				if (this->point && (i == this->point - 1) && (precision != 0)) cout << ".";
			}
		}
		void printall() {
			if (this->all_digit == "") {
				cout << "0" << endl;
				return;
			}
			if (this->sign)cout << "-";
			if (this->point > 0) {
				for (int i = 0; i < this->all_digit.length(); i++) {
					cout << this->all_digit[i];
					if (this->point != this->all_digit.length() && (i == this->point - 1)) cout << ".";
				}
			}
			else {
				cout << "0.";
				for (int i = 0; i < -this->point; i++)
					cout << "0";
				for (int i = 0; i < this->all_digit.length(); i++) {
					cout << this->all_digit[i];
					if (this->point != this->all_digit.length() && (i == this->point - 1)) cout << ".";
				}

			}
			cout << endl;
		}
		void printStat() {
			cout << "DIGITS : [" << all_digit << "], POINT_LOC : [" << point << "], SIGN : [" << sign << "]" << endl;
		}
	}triple;
	typedef class BigInteger64
	{
	private:
		vector<u64>		digits;
		bool			sign;
		bool			initialized;
	protected:
		void makeZero()
		{
			digits.clear();
			digits.push_back(0);
			sign = false;
			init_touch();
		}
		void makeOppSign() {
			sign = !sign;
		}
		bool isZero()
		{
			return (digits.size() == 1) && (digits[0] == 0);
		}
		void initialize() {
			digits.clear();
			sign = false;
			initialized = false;
		}
		void check_exception()
		{
			if (!initialized) throw invalid_argument("BigInteger component not initialized!");
		}
		void init_touch()
		{
			initialized = true;
		}
		u64 convert_string_to_u64(string str)
		{
			u64 result = 0U;
			for (auto i = 0;; i++)
			{
				u64 u = str[i] - '0';
				result += u;
				if (i == str.length() - 1)break;
				result *= 10;
			}
			return result;
		}
		void standardize()
		{
			if (!initialized || digits.empty())
			{
				initialize();
				return;
			}
			if (this->isZero())this->sign = false;
		}
		BigInteger64 make_zero_bint64()
		{
			BigInteger64 binter;
			binter.setValue("0");
			return binter;
		}
		void check_exception_pair(BigInteger64 bint1, BigInteger64 bint2)
		{
			try
			{
				bint1.check_exception();
				bint2.check_exception();
			}
			catch (const invalid_argument& e)
			{
				cout << "NotInitializedException occurred in [source code line : " << __LINE__ << "] : " << e.what() << endl;
				exit(0);
			}
		}
		BigInteger64 absolute(const BigInteger64& bintx)
		{
			BigInteger64 bint = bintx;
			bint.sign = false;
			return bint;
		}
	public:
		BigInteger64()
		{
			initialize();
		}
		BigInteger64(string str)
		{
			initialize();
			this->setValue(str);
		}
		void printStatus()
		{
			cout << "[bint info] DIGIT : [";
			for (auto i = 0; i < this->digits.size(); i++)
			{
				cout << digits[i];
				if (i < this->digits.size() - 1)cout << ",";
			}
			cout << "] SIGN : [" << (this->sign ? "true" : "false") << "] INIT : [" << (this->initialized ? "true" : "false") << "]" << endl;
		}
		bool operator==(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			check_exception_pair(bint1, bint2);
			return (bint1.digits == bint2.digits) && (bint1.sign == bint2.sign) && (bint1.initialized == bint2.initialized);
		}
		bool operator>(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			check_exception_pair(bint1, bint2);
			if (bint1.sign == bint2.sign)
			{
				if (bint1.digits.size() == bint2.digits.size())
				{
					for (auto i = 0; i < bint1.digits.size(); i++) {
						if (bint1.digits[i] == bint2.digits[i]) continue;
						return bint1.sign ^ (bint1.digits[i] > bint2.digits[i]);
					}
					return false;
				}
				return bint1.sign ^ (bint1.digits.size() > bint2.digits.size());
			}
			return bint2.sign;
		}
		bool operator<(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			check_exception_pair(bint1, bint2);
			return bint2 > bint1;
		}
		bool operator<=(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			check_exception_pair(bint1, bint2);
			return (bint1 < bint2) || (bint2 == bint1);
		}
		bool operator>=(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			check_exception_pair(bint1, bint2);
			return (bint1 > bint2) || (bint2 == bint1);
		}
		BigInteger64 operator+(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			BigInteger64			result;
			try
			{
				bint1.check_exception();
				bint2.check_exception();
			}
			catch (const invalid_argument& e)
			{
				cout << "NotInitializedException occurred in [source code line : " << __LINE__ << "] : " << e.what() << endl;
				exit(0);
			}
			//if bint1 or bint2 is zero
			if (bint1.isZero()) return bint2;
			if (bint2.isZero()) return bint1;
			//split operator as sign
			//In case signs of bint1 and bint2 are different
			if (bint1.sign ^ bint2.sign)
			{
				if (bint2.sign)
				{
					//plus + minus
					bint2.makeOppSign();
					return bint1 - bint2;
				}
				else
				{
					//minus + plus
					bint1.makeOppSign();
					return bint2 - bint1;
				}
			}
			result.sign = bint1.sign;

			//Adjust length and point
			size_t longer_len = MAXS(bint1.digits.size(), bint2.digits.size());

			/* ------------------ Point Adjusting ------------------ */
			if (bint1.digits.size() > bint2.digits.size())
			{
				size_t gap = bint1.digits.size() - bint2.digits.size();
				for (auto i = 0; i < gap + 1; i++)
				{
					bint2.digits.insert(bint2.digits.begin(), 0);
				}
				bint1.digits.insert(bint1.digits.begin(), 0);
			}
			else
			{
				size_t gap = bint2.digits.size() - bint1.digits.size();
				for (auto i = 0; i < gap + 1; i++)
				{
					bint1.digits.insert(bint1.digits.begin(), 0);
				}
				bint2.digits.insert(bint2.digits.begin(), 0);
			}

			/* ------------------ Calculation ------------------ */
			vector<u64>		reversed;
			u64				addition = 0U;
			for (i64 i = bint1.digits.size() - 1; i >= 0; i--)
			{
				u64 curr = addition + bint1.digits[i] + bint2.digits[i];
				addition = curr / (BINT_MAX + 1);
				curr %= (BINT_MAX + 1);
				reversed.push_back(curr);
			}
			while (!reversed[reversed.size() - 1])reversed.pop_back();
			for (i64 i = reversed.size() - 1; i >= 0; i--)
				result.digits.push_back(reversed[i]);

			result.init_touch();
			result.standardize();
			return result;
		}
		BigInteger64 operator-(const BigInteger64& opp)
		{
			BigInteger64			bint1 = *this;
			BigInteger64			bint2 = opp;
			BigInteger64			result;
			try
			{
				bint1.check_exception();
				bint2.check_exception();
			}
			catch (const invalid_argument& e)
			{
				cout << "NotInitializedException occurred in [source code line : " << __LINE__ << "] : " << e.what() << endl;
				exit(0);
			}
			//if bint1 == bint2
			if (bint1 == bint2) return make_zero_bint64();
			//if bint1 or bint2 is zero
			if (bint1.isZero())
			{
				bint2.makeOppSign();
				return bint2;
			}
			if (bint2.isZero()) return bint1;
			//split operator as sign
			//In case signs of bint1 and bint2 are different
			if (bint1.sign ^ bint2.sign)
			{
				if (bint2.sign)
				{
					//plus - minus
					bint2.makeOppSign();
					return bint1 + bint2;
				}
				else
				{
					//minus - plus
					bint1.makeOppSign();
					BigInteger64 bint3 = bint1 + bint2;
					bint3.makeOppSign();
					return bint3;
				}
			}
			else
			{
				if (absolute(bint1) > absolute(bint2))
				{
					if (bint1.sign)
					{
						BigInteger64 bint3 = absolute(bint1) - absolute(bint2);
						bint3.makeOppSign();
						return bint3;
					}
				}
				else
				{
					BigInteger64 bint3 = absolute(bint2) - absolute(bint1);
					if (!bint1.sign) bint3.makeOppSign();
					return bint3;
				}
			}
			result.sign = false;
			//Adjust length and point

			/* ------------------ Point Adjusting ------------------ */
			size_t gap = bint1.digits.size() - bint2.digits.size();
			for (auto i = 0; i < gap; i++)
				bint2.digits.insert(bint2.digits.begin(), 0);

			/* ------------------ Calculation ------------------ */
			vector<u64>		reversed;
			u64				subtraction = 0U;
			for (i64 i = bint1.digits.size() - 1; i >= 0; i--)
			{
				u64 plus = bint1.digits[i];
				u64 minus = bint2.digits[i] + subtraction;
				u64 curr = 0U;
				if (plus >= minus)
				{
					subtraction = 0U;
					curr = plus - minus;
				}
				else
				{
					subtraction = 1U;
					curr = (plus + BINT_MAX + 1) - minus;
				}
				reversed.push_back(curr);
			}
			while (!reversed[reversed.size() - 1])reversed.pop_back();
			for (i64 i = reversed.size() - 1; i >= 0; i--)
				result.digits.push_back(reversed[i]);

			result.init_touch();
			result.standardize();
			return result;
		}
		void setValue(string str)
		{
			initialize();
			//clean string
			if (str[0] == '-')
			{
				this->sign = true;
				str.erase(str.begin());
			}
			u64 lensize = str.length() / BINT_MAX_DIGIT;
			u64 remains = str.length() % BINT_MAX_DIGIT;
			if (remains > 0)
			{
				u64 u = convert_string_to_u64(str.substr(0, remains));
				str = str.substr(remains, str.length() - remains);
				this->digits.push_back(u);
			}
			for (auto i = 0; i < lensize; i++)
			{
				u64 u = convert_string_to_u64(str.substr(i*BINT_MAX_DIGIT, BINT_MAX_DIGIT));
				this->digits.push_back(u);
			}
			this->init_touch();
		}
		void express()
		{
			this->standardize();
			try
			{
				this->check_exception();
				if (this->sign)cout << "-";
				cout << this->digits[0];
				for (auto i = 1; i < this->digits.size(); i++)
					cout << setw(BINT_MAX_DIGIT) << setfill('0') << this->digits[i];
				cout << "\n";
			}
			catch (const invalid_argument& e)
			{
				cout << "NotInitializedException occurred in [source code line : " << __LINE__ << "] : " << e.what() << endl;
				exit(0);
			}
		}
	}bint64;
	bint64 new_bint64(string str) {
		bint64 bint(str);
		return bint;
	}
}using namespace newtypes;

int main() {
	bint64 b1, b2, b3;
	b1.setValue("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
	b2.setValue("52");
	b1.express();
	b2.express();
	b3 = b1 + b2;
	b3.express();
}