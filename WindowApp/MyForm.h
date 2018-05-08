#pragma once

namespace CppWinForm3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{

	delegate void TextBoxesDelegate(System::Single^ set, System::Single^ ind); // needed to allow "prot monitoring thread" to change textBoxes in "MyForm thread"

	delegate void GetParametersDelegate(System::Single^ Kp, System::Single^ Ki, System::Single^ Kd); // needed to allow "prot monitoring thread" to change numericUpDowns in "MyForm thread"

	delegate void EnableGetButtonDelegate(); // needed to allow "prot monitoring thread" to enable 'get parameters' button in "MyForm thread"

	private:
		bool expectingParameters = false;
		bool timeOut = false;
		System::Single^ setTemp;
		System::Single^ indTemp;
		System::Int16^ timeIncrement;
		System::IO::StreamWriter^ dataFile;


	public:
		MyForm(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private: System::Windows::Forms::TextBox^  textBox_setpoint;
	private: System::Windows::Forms::Label^  label_setpoint;
	private: System::Windows::Forms::Button^  button_register;
	private: System::Windows::Forms::Button^  button_stop;
	private: System::Windows::Forms::Button^  button_open;
	private: System::Windows::Forms::Button^  button_close;
	private: System::Windows::Forms::Label^  label_temp;
	private: System::Windows::Forms::Label^  label_status;
	private: System::Windows::Forms::Label^  label_COM;
	private: System::Windows::Forms::TextBox^  textBox_temp;
	private: System::Windows::Forms::TextBox^  textBox_state;
	private: System::Windows::Forms::ComboBox^  comboBox_COMports;
	private: System::IO::Ports::SerialPort^  arduinoSerialPort;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_Kp;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_Ki;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_Kd;
	private: System::Windows::Forms::Label^  label_Kp;
	private: System::Windows::Forms::Label^  label_Ki;
	private: System::Windows::Forms::Label^  label_Kd;
	private: System::Windows::Forms::Button^  button_getParameters;
	private: System::Windows::Forms::Button^  button_setParameters;
	private: System::Windows::Forms::Button^  button_showDefaults;
	private: System::Windows::Forms::Timer^  timer500;
	private: System::Windows::Forms::Timer^  timer1000;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->button_open = (gcnew System::Windows::Forms::Button());
			this->button_close = (gcnew System::Windows::Forms::Button());
			this->comboBox_COMports = (gcnew System::Windows::Forms::ComboBox());
			this->label_temp = (gcnew System::Windows::Forms::Label());
			this->label_status = (gcnew System::Windows::Forms::Label());
			this->arduinoSerialPort = (gcnew System::IO::Ports::SerialPort(this->components));
			this->label_COM = (gcnew System::Windows::Forms::Label());
			this->textBox_temp = (gcnew System::Windows::Forms::TextBox());
			this->textBox_state = (gcnew System::Windows::Forms::TextBox());
			this->numericUpDown_Kp = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown_Ki = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown_Kd = (gcnew System::Windows::Forms::NumericUpDown());
			this->label_Kp = (gcnew System::Windows::Forms::Label());
			this->label_Ki = (gcnew System::Windows::Forms::Label());
			this->label_Kd = (gcnew System::Windows::Forms::Label());
			this->button_getParameters = (gcnew System::Windows::Forms::Button());
			this->button_setParameters = (gcnew System::Windows::Forms::Button());
			this->button_showDefaults = (gcnew System::Windows::Forms::Button());
			this->timer500 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer1000 = (gcnew System::Windows::Forms::Timer(this->components));
			this->textBox_setpoint = (gcnew System::Windows::Forms::TextBox());
			this->label_setpoint = (gcnew System::Windows::Forms::Label());
			this->button_register = (gcnew System::Windows::Forms::Button());
			this->button_stop = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_Kp))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_Ki))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_Kd))->BeginInit();
			this->SuspendLayout();
			// 
			// button_open
			// 
			this->button_open->Location = System::Drawing::Point(125, 33);
			this->button_open->Name = L"button_open";
			this->button_open->Size = System::Drawing::Size(75, 23);
			this->button_open->TabIndex = 0;
			this->button_open->Text = L"open";
			this->button_open->UseVisualStyleBackColor = true;
			this->button_open->Click += gcnew System::EventHandler(this, &MyForm::button_start_Click);
			// 
			// button_close
			// 
			this->button_close->Enabled = false;
			this->button_close->Location = System::Drawing::Point(206, 33);
			this->button_close->Name = L"button_close";
			this->button_close->Size = System::Drawing::Size(75, 23);
			this->button_close->TabIndex = 1;
			this->button_close->Text = L"close";
			this->button_close->UseVisualStyleBackColor = true;
			this->button_close->Click += gcnew System::EventHandler(this, &MyForm::button_stop_Click);
			// 
			// comboBox_COMports
			// 
			this->comboBox_COMports->FormattingEnabled = true;
			this->comboBox_COMports->Location = System::Drawing::Point(27, 33);
			this->comboBox_COMports->Name = L"comboBox_COMports";
			this->comboBox_COMports->Size = System::Drawing::Size(75, 21);
			this->comboBox_COMports->TabIndex = 2;
			this->comboBox_COMports->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox_COMports_SelectedIndexChanged);
			this->comboBox_COMports->Click += gcnew System::EventHandler(this, &MyForm::comboBox_COMports_Click);
			// 
			// label_temp
			// 
			this->label_temp->AutoSize = true;
			this->label_temp->Location = System::Drawing::Point(24, 155);
			this->label_temp->Name = L"label_temp";
			this->label_temp->Size = System::Drawing::Size(67, 13);
			this->label_temp->TabIndex = 3;
			this->label_temp->Text = L"Temperature";
			// 
			// label_status
			// 
			this->label_status->AutoSize = true;
			this->label_status->Location = System::Drawing::Point(24, 75);
			this->label_status->Name = L"label_status";
			this->label_status->Size = System::Drawing::Size(37, 13);
			this->label_status->TabIndex = 4;
			this->label_status->Text = L"Status";
			// 
			// arduinoSerialPort
			// 
			this->arduinoSerialPort->PortName = L"COM6";
			this->arduinoSerialPort->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &MyForm::arduinoSerialPort_DataReceived);
			// 
			// label_COM
			// 
			this->label_COM->AutoSize = true;
			this->label_COM->Location = System::Drawing::Point(36, 17);
			this->label_COM->Name = L"label_COM";
			this->label_COM->Size = System::Drawing::Size(52, 13);
			this->label_COM->TabIndex = 5;
			this->label_COM->Text = L"COM port";
			// 
			// textBox_temp
			// 
			this->textBox_temp->Enabled = false;
			this->textBox_temp->Location = System::Drawing::Point(98, 152);
			this->textBox_temp->Name = L"textBox_temp";
			this->textBox_temp->Size = System::Drawing::Size(100, 20);
			this->textBox_temp->TabIndex = 6;
			this->textBox_temp->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_state
			// 
			this->textBox_state->Location = System::Drawing::Point(62, 72);
			this->textBox_state->Name = L"textBox_state";
			this->textBox_state->Size = System::Drawing::Size(219, 20);
			this->textBox_state->TabIndex = 7;
			// 
			// numericUpDown_Kp
			// 
			this->numericUpDown_Kp->DecimalPlaces = 3;
			this->numericUpDown_Kp->Enabled = false;
			this->numericUpDown_Kp->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->numericUpDown_Kp->Location = System::Drawing::Point(98, 205);
			this->numericUpDown_Kp->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericUpDown_Kp->Name = L"numericUpDown_Kp";
			this->numericUpDown_Kp->Size = System::Drawing::Size(59, 20);
			this->numericUpDown_Kp->TabIndex = 8;
			this->numericUpDown_Kp->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// numericUpDown_Ki
			// 
			this->numericUpDown_Ki->DecimalPlaces = 3;
			this->numericUpDown_Ki->Enabled = false;
			this->numericUpDown_Ki->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->numericUpDown_Ki->Location = System::Drawing::Point(98, 235);
			this->numericUpDown_Ki->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericUpDown_Ki->Name = L"numericUpDown_Ki";
			this->numericUpDown_Ki->Size = System::Drawing::Size(59, 20);
			this->numericUpDown_Ki->TabIndex = 9;
			this->numericUpDown_Ki->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// numericUpDown_Kd
			// 
			this->numericUpDown_Kd->DecimalPlaces = 3;
			this->numericUpDown_Kd->Enabled = false;
			this->numericUpDown_Kd->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->numericUpDown_Kd->Location = System::Drawing::Point(98, 265);
			this->numericUpDown_Kd->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericUpDown_Kd->Name = L"numericUpDown_Kd";
			this->numericUpDown_Kd->Size = System::Drawing::Size(59, 20);
			this->numericUpDown_Kd->TabIndex = 10;
			this->numericUpDown_Kd->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label_Kp
			// 
			this->label_Kp->AutoSize = true;
			this->label_Kp->Location = System::Drawing::Point(59, 207);
			this->label_Kp->Name = L"label_Kp";
			this->label_Kp->Size = System::Drawing::Size(20, 13);
			this->label_Kp->TabIndex = 11;
			this->label_Kp->Text = L"Kp";
			// 
			// label_Ki
			// 
			this->label_Ki->AutoSize = true;
			this->label_Ki->Location = System::Drawing::Point(59, 237);
			this->label_Ki->Name = L"label_Ki";
			this->label_Ki->Size = System::Drawing::Size(16, 13);
			this->label_Ki->TabIndex = 12;
			this->label_Ki->Text = L"Ki";
			// 
			// label_Kd
			// 
			this->label_Kd->AutoSize = true;
			this->label_Kd->Location = System::Drawing::Point(59, 267);
			this->label_Kd->Name = L"label_Kd";
			this->label_Kd->Size = System::Drawing::Size(20, 13);
			this->label_Kd->TabIndex = 13;
			this->label_Kd->Text = L"Kd";
			// 
			// button_getParameters
			// 
			this->button_getParameters->Enabled = false;
			this->button_getParameters->Location = System::Drawing::Point(185, 202);
			this->button_getParameters->Name = L"button_getParameters";
			this->button_getParameters->Size = System::Drawing::Size(96, 23);
			this->button_getParameters->TabIndex = 14;
			this->button_getParameters->Text = L"get parameters";
			this->button_getParameters->UseVisualStyleBackColor = true;
			this->button_getParameters->Click += gcnew System::EventHandler(this, &MyForm::button_getParameters_Click);
			// 
			// button_setParameters
			// 
			this->button_setParameters->Enabled = false;
			this->button_setParameters->Location = System::Drawing::Point(185, 262);
			this->button_setParameters->Name = L"button_setParameters";
			this->button_setParameters->Size = System::Drawing::Size(96, 23);
			this->button_setParameters->TabIndex = 15;
			this->button_setParameters->Text = L"set parameters";
			this->button_setParameters->UseVisualStyleBackColor = true;
			this->button_setParameters->Click += gcnew System::EventHandler(this, &MyForm::button_setParameters_Click);
			// 
			// button_showDefaults
			// 
			this->button_showDefaults->Enabled = false;
			this->button_showDefaults->Location = System::Drawing::Point(185, 232);
			this->button_showDefaults->Name = L"button_showDefaults";
			this->button_showDefaults->Size = System::Drawing::Size(96, 23);
			this->button_showDefaults->TabIndex = 16;
			this->button_showDefaults->Text = L"show defaults";
			this->button_showDefaults->UseVisualStyleBackColor = true;
			this->button_showDefaults->Click += gcnew System::EventHandler(this, &MyForm::button_getDefaults_Click);
			// 
			// timer500
			// 
			this->timer500->Interval = 500;
			this->timer500->Tick += gcnew System::EventHandler(this, &MyForm::timer500_Tick);
			// 
			// timer1000
			// 
			this->timer1000->Interval = 1000;
			this->timer1000->Tick += gcnew System::EventHandler(this, &MyForm::timer1000_Tick);
			// 
			// textBox_setpoint
			// 
			this->textBox_setpoint->Enabled = false;
			this->textBox_setpoint->Location = System::Drawing::Point(98, 121);
			this->textBox_setpoint->Name = L"textBox_setpoint";
			this->textBox_setpoint->Size = System::Drawing::Size(100, 20);
			this->textBox_setpoint->TabIndex = 18;
			this->textBox_setpoint->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label_setpoint
			// 
			this->label_setpoint->AutoSize = true;
			this->label_setpoint->Location = System::Drawing::Point(24, 124);
			this->label_setpoint->Name = L"label_setpoint";
			this->label_setpoint->Size = System::Drawing::Size(46, 13);
			this->label_setpoint->TabIndex = 17;
			this->label_setpoint->Text = L"Setpoint";
			// 
			// button_register
			// 
			this->button_register->Enabled = false;
			this->button_register->Location = System::Drawing::Point(206, 119);
			this->button_register->Name = L"button_register";
			this->button_register->Size = System::Drawing::Size(75, 23);
			this->button_register->TabIndex = 19;
			this->button_register->Text = L"register";
			this->button_register->UseVisualStyleBackColor = true;
			this->button_register->Click += gcnew System::EventHandler(this, &MyForm::button_register_Click);
			// 
			// button_stop
			// 
			this->button_stop->Enabled = false;
			this->button_stop->Location = System::Drawing::Point(206, 150);
			this->button_stop->Name = L"button_stop";
			this->button_stop->Size = System::Drawing::Size(75, 23);
			this->button_stop->TabIndex = 20;
			this->button_stop->Text = L"stop";
			this->button_stop->UseVisualStyleBackColor = true;
			this->button_stop->Click += gcnew System::EventHandler(this, &MyForm::button_stop_Click_1);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(309, 306);
			this->Controls->Add(this->button_stop);
			this->Controls->Add(this->button_register);
			this->Controls->Add(this->textBox_setpoint);
			this->Controls->Add(this->label_setpoint);
			this->Controls->Add(this->button_showDefaults);
			this->Controls->Add(this->button_setParameters);
			this->Controls->Add(this->button_getParameters);
			this->Controls->Add(this->label_Kd);
			this->Controls->Add(this->label_Ki);
			this->Controls->Add(this->label_Kp);
			this->Controls->Add(this->numericUpDown_Kd);
			this->Controls->Add(this->numericUpDown_Ki);
			this->Controls->Add(this->numericUpDown_Kp);
			this->Controls->Add(this->textBox_state);
			this->Controls->Add(this->textBox_temp);
			this->Controls->Add(this->label_COM);
			this->Controls->Add(this->label_status);
			this->Controls->Add(this->label_temp);
			this->Controls->Add(this->comboBox_COMports);
			this->Controls->Add(this->button_close);
			this->Controls->Add(this->button_open);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(325, 345);
			this->MinimumSize = System::Drawing::Size(325, 345);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Soldering station";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm::MyForm_FormClosed);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_Kp))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_Ki))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown_Kd))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void comboBox_COMports_Click(System::Object^  sender, System::EventArgs^  e) {
		array<System::String^>^ serialPorts = nullptr;
		serialPorts = arduinoSerialPort->GetPortNames();
		
		this->comboBox_COMports->Items->Clear();
		
		for each(String^ port in serialPorts)
		{
			this->comboBox_COMports->Items->Add(port);
		}
	}

	private: System::Void comboBox_COMports_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		if (arduinoSerialPort->IsOpen) {

			arduinoSerialPort->Close();
			textBox_state->Text = "Closed port " + arduinoSerialPort->PortName;

			button_open->Enabled = true;
			button_close->Enabled = false;

			textBox_setpoint->Text = "";
			textBox_temp->Text = "";
			textBox_setpoint->Enabled = false;
			textBox_temp->Enabled = false;

			button_register->Enabled = false;

			numericUpDown_Kp->Enabled = false;
			numericUpDown_Ki->Enabled = false;
			numericUpDown_Kd->Enabled = false;
			numericUpDown_Kp->Value = Convert::ToDecimal(0);
			numericUpDown_Ki->Value = Convert::ToDecimal(0);
			numericUpDown_Kd->Value = Convert::ToDecimal(0);
			
			button_getParameters->Enabled = false;
			button_showDefaults->Enabled = false;
			button_setParameters->Enabled = false;
		}
	}

	private: System::Void button_start_Click(System::Object^  sender, System::EventArgs^  e) {
		if (!arduinoSerialPort->IsOpen)
		{
			try
			{
				arduinoSerialPort->PortName = this->comboBox_COMports->SelectedItem->ToString();
				arduinoSerialPort->BaudRate = 9600;

				try
				{
					arduinoSerialPort->Open();

					textBox_state->Text = "Opened port " + arduinoSerialPort->PortName;

					button_open->Enabled = false;
					button_close->Enabled = true;

					textBox_setpoint->Enabled = true;
					textBox_temp->Enabled = true;

					button_register->Enabled = true;

					numericUpDown_Kp->Enabled = true;
					numericUpDown_Ki->Enabled = true;
					numericUpDown_Kd->Enabled = true;
					
					button_getParameters->Enabled = true;
					button_showDefaults->Enabled = true;
					button_setParameters->Enabled = true;
				}
				catch (System::Exception^ e)
				{
					textBox_state->Text = e->Message;
				}
			}
			catch (System::Exception^ e)
			{
				textBox_state->Text = "No port selected";
			}
		}
	}

	private: System::Void button_stop_Click(System::Object^  sender, System::EventArgs^  e) {
		arduinoSerialPort->Close();

		textBox_state->Text = "Closed port " + arduinoSerialPort->PortName;

		button_open->Enabled = true;
		button_close->Enabled = false;

		textBox_setpoint->Text = "";
		textBox_temp->Text = "";
		textBox_setpoint->Enabled = false;
		textBox_temp->Enabled = false;

		button_register->Enabled = false;

		numericUpDown_Kp->Enabled = false;
		numericUpDown_Ki->Enabled = false;
		numericUpDown_Kd->Enabled = false;
		numericUpDown_Kp->Value = Convert::ToDecimal(0);
		numericUpDown_Ki->Value = Convert::ToDecimal(0);
		numericUpDown_Kd->Value = Convert::ToDecimal(0);
		
		button_getParameters->Enabled = false;
		button_showDefaults->Enabled = false;
		button_setParameters->Enabled = false;
	}

	private: System::Void printTemps(System::Single^ set, System::Single^ ind) {
		if (this->textBox_temp->InvokeRequired) {
			TextBoxesDelegate^ d = gcnew TextBoxesDelegate(this, &MyForm::printTemps);
			this->Invoke(d, gcnew array<Object^> {set,ind});
		}
		else {
			textBox_setpoint->Text = set +" °C";
			textBox_temp->Text = ind->ToString("F2") + " °C";
		}
	}

	private: System::Void setParameters(System::Single^ Kp, System::Single^ Ki, System::Single^ Kd) {
		if (this->numericUpDown_Kp->InvokeRequired) {
			GetParametersDelegate^ d = gcnew GetParametersDelegate(this, &MyForm::setParameters);
			this->Invoke(d, gcnew array<Object^> {Kp,Ki,Kd});
		}
		else {
			try {
				numericUpDown_Kp->Value = Convert::ToDecimal(Kp);
				numericUpDown_Ki->Value = Convert::ToDecimal(Ki);
				numericUpDown_Kd->Value = Convert::ToDecimal(Kd);
			}
			catch (System::Exception^ e) {/* do nothing */}
		}
	}

	private: System::Void enableGetButton() {
		if (this->button_getParameters->InvokeRequired) {
			EnableGetButtonDelegate^ d = gcnew EnableGetButtonDelegate(this, &MyForm::enableGetButton);
			this->Invoke(d, gcnew array<Object^> {});
		}
		else {
			button_getParameters->Enabled = true;
		}
	}

	private: System::Void arduinoSerialPort_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {

		array<Byte>^ readBytes = gcnew array<Byte>(4);

		if (expectingParameters) {

			timer1000->Enabled = true; // switching timeOut flag after 1s

			while (arduinoSerialPort->BytesToRead < 12 && !timeOut) {}

			if (timeOut) { // abort attempt and clear data buffer
				int buffetLength = arduinoSerialPort->BytesToRead;
				for (buffetLength; buffetLength > 0; buffetLength--) { arduinoSerialPort->ReadByte(); }
			}
			else { // read received data
				for (int i = 0; i < 4; i++) readBytes[i] = arduinoSerialPort->ReadByte();
				System::Single^ Kp = gcnew System::Single(BitConverter::ToSingle(readBytes, 0));
				for (int i = 0; i < 4; i++) readBytes[i] = arduinoSerialPort->ReadByte();
				System::Single^ Ki = gcnew System::Single(BitConverter::ToSingle(readBytes, 0));
				for (int i = 0; i < 4; i++) readBytes[i] = arduinoSerialPort->ReadByte();
				System::Single^ Kd = gcnew System::Single(BitConverter::ToSingle(readBytes, 0));

				this->setParameters(Kp, Ki, Kd);
			}

			enableGetButton(); // enable 'get parameters' button after receive attempt
			expectingParameters = false;
		}
		else { // not expecting parameters transmission from arduino (reading temperatures)

			timer1000->Enabled = true; // switching timeOut flag after 1s
			
			while ((arduinoSerialPort->BytesToRead < 10) && (!timeOut)) {}

			if (timeOut) { // abort attempt and clear data buffer
				int buffetLength = arduinoSerialPort->BytesToRead;
				for (buffetLength; buffetLength > 0; buffetLength--) { arduinoSerialPort->ReadByte(); }
			}
			else { // read received data
				for (int i = 0; i < 2; i++) readBytes[i] = arduinoSerialPort->ReadByte();
				timeIncrement = gcnew System::Int16(BitConverter::ToInt32(readBytes, 0));
				for (int i = 0; i < 4; i++) readBytes[i] = arduinoSerialPort->ReadByte();
				setTemp = gcnew System::Single(BitConverter::ToSingle(readBytes, 0));
				for (int i = 0; i < 4; i++) readBytes[i] = arduinoSerialPort->ReadByte();
				indTemp = gcnew System::Single(BitConverter::ToSingle(readBytes, 0));

				this->printTemps(setTemp, indTemp);

				// writing to file
				if (button_stop->Enabled == true) {
					String^ line = String::Format("{0}\t{1}\t{2}",timeIncrement->ToString(),setTemp->ToString(),indTemp->ToString("F2"));
					dataFile->WriteLine(line);
				}
			}
		}
	}

	private: System::Void MyForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
		arduinoSerialPort->Close();
		Application::Exit();
	}

	private: System::Void button_getParameters_Click(System::Object^  sender, System::EventArgs^  e) {
		if (arduinoSerialPort->IsOpen)
		{
			arduinoSerialPort->Write("T"); // transfer request flag for arduino
			expectingParameters = true; // expecting data from arduino
			button_getParameters->Enabled = false; // disable 'get parameters' button for the time of transmission
		}
	}

	private: System::Void button_getDefaults_Click(System::Object^  sender, System::EventArgs^  e) {
		numericUpDown_Kp->Value = Convert::ToDecimal(4);
		numericUpDown_Ki->Value = Convert::ToDecimal(0.04);
		numericUpDown_Kd->Value = Convert::ToDecimal(0);
	}

	private: System::Void button_setParameters_Click(System::Object^  sender, System::EventArgs^  e) {
		if (arduinoSerialPort->IsOpen)
		{
			arduinoSerialPort->Write("R"); // receive request flag for arduino

			button_setParameters->Enabled = false; // disable 'set parameters' button for 0.5s
			timer500->Enabled = true; // enables 'set parameters' button in 0.5s

			array<Byte>^ toSend = System::BitConverter::GetBytes(Convert::ToSingle(numericUpDown_Kp->Value));
			arduinoSerialPort->Write(toSend, 0, 4);

			toSend = System::BitConverter::GetBytes(Convert::ToSingle(numericUpDown_Ki->Value));
			arduinoSerialPort->Write(toSend, 0, 4);

			toSend = System::BitConverter::GetBytes(Convert::ToSingle(numericUpDown_Kd->Value));
			arduinoSerialPort->Write(toSend, 0, 4);
		}
	}

	private: System::Void timer500_Tick(System::Object^  sender, System::EventArgs^  e) {
		if (button_setParameters->Enabled == false) button_setParameters->Enabled = true;
		timer500->Enabled = false;
	}

	private: System::Void timer1000_Tick(System::Object^  sender, System::EventArgs^  e) {
		timeOut = true;
	}

	private: System::Void button_register_Click(System::Object^  sender, System::EventArgs^  e) {
		dataFile = gcnew System::IO::StreamWriter("soldering_station_register.txt",0);
		// '0' in constructor means that new file is going to be created in stead of appending the old one
		button_register->Enabled = false;
		button_stop->Enabled = true;
	}

	private: System::Void button_stop_Click_1(System::Object^  sender, System::EventArgs^  e) {
		dataFile->Close();
		button_register->Enabled = true;
		button_stop->Enabled = false;
	}

};
}