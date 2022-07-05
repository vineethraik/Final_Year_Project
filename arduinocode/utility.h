#include "libs.h"

//'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',



class utility {

public:
  String get_hash(String);
  String get_pin_from_hex(String);
  String get_rand_string(int);
  String get_rand_otp(int);

} util;

String utility::get_hash(String str) {
  String s = sha1(str);
  s.toUpperCase();
  return s;
}

String utility::get_pin_from_hex(String _hex) {
  int _pin[] = { 0, 0, 0, 0, 0, 0 };
  _hex.toUpperCase();
  String C = _hex;
  String pin = "";
  for (int i = 0; i < _hex.length(); i++) {
    switch (C[i]) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        _pin[i % 6] += C[i] - 48;
        break;
      case 'A':
        _pin[i % 6] += 10;
        break;
      case 'B':
        _pin[i % 6] += 11;
        break;
      case 'C':
        _pin[i % 6] += 12;
        break;
      case 'D':
        _pin[i % 6] += 13;
        break;
      case 'E':
        _pin[i % 6] += 14;
        break;
      case 'F':
        _pin[i % 6] += 15;
        break;
    }
    _pin[i % 6] = _pin[i % 6] % 10;
  }

  for (int i = 0; i < 6; i++) {
    pin += String(_pin[i]);
  }

  return pin;
}

String utility::get_rand_string(int len = 16) {
  char c_store[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
  };
  String s = "";
  int n = sizeof(c_store) / sizeof(c_store[0]);
  for (int i = 0; i < len; i++) {
    s += c_store[random(n)];
  }
  return s;
}

String utility::get_rand_otp(int n = 6) {
  String otp = "";
  while (n--) {
    otp += String(random(0, 50000) % 10);
  }
  return otp;
}