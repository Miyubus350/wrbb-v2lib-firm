/*
 * WRBB Main
 *
 * Copyright (c) 2016 Wakayama.rb Ruby Board developers
 *
 * This software is released under the MIT License.
 * https://github.com/wakayamarb/wrbb-v2lib-firm/blob/master/MITL
 *
 */
#include <Arduino.h>

#include <eepfile.h>
#include <eeploader.h>
#include <iodefine_gcc63n.h>

#include <mruby/version.h>
#include <mruby.h>

#include <sExec.h>
#include "wrbb.h"

char RubyStartFileName[RUBY_FILENAME_SIZE];	//xml�Ɏw�肳�ꂽ�ŏ��ɋN������mruby�t�@�C����
char RubyFilename[RUBY_FILENAME_SIZE];
char ExeFilename[RUBY_FILENAME_SIZE];		//���ݎ��s����Ă���t�@�C���̃p�X��

extern volatile char ProgVer[];

//**********************************
//���������s���܂�
//**********************************
void init_vm( void )
{
char dat[4];
int en;
int i;

	//EEP�t�@�C���֘A�̏�����
	EEP.begin();

	//�X�^�[�g�t�@�C������ǂݍ��݂܂�
	RubyStartFileName[0] = 0;

	FILEEEP fpj;
	FILEEEP *fp = &fpj;

	//�X�^�[�g�t�@�C������ǂݍ��݂܂�
	if(EEP.fopen( fp, XML_FILENAME, EEP_READ ) == -1){
		strcpy( RubyStartFileName, RUBY_FILENAME );
	}
	else{
		//file �� ack ���o��܂łЂ�����ǂݍ��݂܂�
		int pos = 0;
		while( !EEP.fEof(fp) ){
			EEP.fseek(fp, pos, EEP_SEEKTOP);

			dat[0] = 0;
			dat[1] = 0;
			dat[2] = 0;
			dat[3] = 0;
			for( i=0; i< 4; i++ ){
				en = EEP.fread(fp);
				if( en<0 ){ break; }
				dat[i] = (char)en;
			}

			if( RubyStartFileName[0]==0 && dat[0]=='f' && dat[1]=='i' && dat[2]=='l' && dat[3]=='e'  ){

				//���������̂� " or ' �܂œǂݔ�΂�
				while( !EEP.fEof(fp) ){
					en = EEP.fread(fp);
					if( en<0 ){ break; }
					if( (char)en==0x22 || (char)en==0x27 ){

						//���������̂� " or ' �܂Ŏ�荞�݂܂�
						for( i=0; i<EEPFILENAME_SIZE; i++ ){
							en = EEP.fread(fp);
							if( en<0 ){
								RubyStartFileName[0] = 0;
								break;
							}

							if( (char)en==0x22 || (char)en==0x27 ){
								RubyStartFileName[i] = 0;
								break;
							}
							RubyStartFileName[i] = (char)en;
						}
						break;
					}
				}
			}
			pos++;
		}
		EEP.fclose(fp);
	}

	if(RubyStartFileName[0] == 0){
		strcpy( RubyStartFileName, RUBY_FILENAME );
	}

	//RubyFilename�ɃX�^�[�g�t�@�C�������R�s�[���܂�
	strcpy( RubyFilename, RubyStartFileName );
}

//**************************************************
// �Z�b�g�A�b�v���܂�
//**************************************************
void setup()
{
    pinMode(RB_LED, OUTPUT);

#if BOARD == BOARD_GR
    pinMode(PIN_LED0, OUTPUT);
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);
#endif

	//�s�����[�h����͂ɏ��������܂�
	pinModeInit();
	
	//�V���A���ʐM�̏�����
	Serial.begin(115200);		//���zUSB�V���A��
    //Serial.setDefault();
	//sci_convert_crlf_ex(Serial.get_handle(), CRLF_NONE, CRLF_NONE);	//�o�C�i����ʂ���悤�ɂ���

	//vm�̏�����
	init_vm();

	//Port 3-5��HIGH��������AEEPROM�t�@�C�����[�_�[�ɔ��
	if( FILE_LOAD == 1 ){
		fileloader((const char*)ProgVer,MRUBY_VERSION);
	}
}

//**************************************************
// �������[�v�ł�
//**************************************************
void loop()
{
	if( RubyRun()==false ){

		DEBUG_PRINT("RubyRun", "FALSE");
		fileloader((const char*)ProgVer,MRUBY_VERSION);
	}
}