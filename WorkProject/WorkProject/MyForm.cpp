#include "MyForm.h"

using namespace WorkProject;

[STAThreadAttribute]

int main(void)
{
	//true == 1, false == 0
	// �إߥ��󱱨���e�A���ҥ� Windows XP ��ı�ƮĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// �إߥD�����ð���
	Application::Run(gcnew MyForm());
	//�����{������A�æ^�� 0 �N���{�����楿�`�L�����C
	return 0;
}
