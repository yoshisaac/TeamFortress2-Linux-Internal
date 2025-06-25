#ifndef KEYVALUES_HPP
#define KEYVALUES_HPP

#include <string.h>

enum types_t {
  TYPE_NONE = 0,
  TYPE_STRING,
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_PTR,
  TYPE_WSTRING,
  TYPE_COLOR,
  TYPE_UINT64,
  TYPE_NUMTYPES,
};

class KeyValues {

  KeyValues* FindKey(const char *keyName, bool bCreate);
  
  void SetString(const char* keyName, const char* value) {
    KeyValues* dat = FindKey(keyName, true);

    if (dat) {
      if (dat->m_iDataType == TYPE_STRING && dat->m_sValue == value) {
	return;
      }

      delete[] dat->m_sValue;
      delete[] dat->m_wsValue;
      dat->m_wsValue = NULL;

      if (!value) {
	value = "";
      }

      int len = int(strlen(value));
      dat->m_sValue = new char[len + 1];
      memcpy(dat->m_sValue, value, len + 1);

      dat->m_iDataType = TYPE_STRING;
    }
  }

private:
  int m_iKeyName;
  char* m_sValue;
  wchar_t* m_wsValue;

  union {
    int m_iValue;
    float m_flValue;
    void* m_pValue;
    unsigned char m_Color[4];
  };

  char m_iDataType;
  char m_bHasEscapeSequences;
  char m_bEvaluateConditionals;
  char unused[1];

  KeyValues* m_pPeer;
  KeyValues* m_pSub;
  KeyValues* m_pChain;
};

#endif
