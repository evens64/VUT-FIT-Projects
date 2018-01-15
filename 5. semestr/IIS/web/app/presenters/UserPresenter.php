<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Nette\Database;

class UserPresenter extends BasePresenter
{
	private $database;

	public function __construct(Nette\Database\Context $database){
		
		$this->database = $database;
	}

    public function renderDefault() {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
        		$this->redirect('Homepage:Default');
    	}

		$this->template->postavy_zive = $this->database->query("SELECT * FROM postavy WHERE ID_hrace=? AND ID_postavy NOT IN (SELECT ID_postavy FROM mn_umrti) ORDER BY stat_uroven DESC", $this->user->identity->ID_hrace);

		$this->template->postavy_mrtve = $this->database->query("SELECT * FROM postavy WHERE ID_hrace=? AND ID_postavy IN (SELECT ID_postavy FROM mn_umrti) ORDER BY stat_uroven DESC", $this->user->identity->ID_hrace);
	}
	
	public function renderSittings() {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
        		$this->redirect('Homepage:Default');
    	}
		
	}

	public function renderSettings() {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
        		$this->redirect('Homepage:Default');
    	}

		$this->template->zadost = $this->database
									->query("SELECT datum_vytvoreni, priznak FROM zadosti
									WHERE ID_hrace=? 
									AND datum_vytvoreni=
									(SELECT MAX(datum_vytvoreni) FROM zadosti WHERE ID_hrace=?)",
									$this->user->identity->ID_hrace,$this->user->identity->ID_hrace)
									->fetch();
    }

	// Change nickname
	//******************************************************
	protected function createComponentChangeNicknameForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeNicknameFormSucceeded'];
        return $form;

    }
    
    public function changeNicknameFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['prezdivka'] = $form->getHttpData($form::DATA_LINE, 'prezdivka');

		try{	
			$result = $this->database->table('hraci')->where("uziv_jmeno= ? ",$this->user->identity->uziv_jmeno)->update($values);
			$this->user->identity->prezdivka = $values['prezdivka'];
			$this->flashMessage("Byla provedena změna přezdívky.");
			$this->redirect('User:Settings');
		} catch(Nette\Database\DriverException $exception){
			$str = $exception->getMessage();
			$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
			$this->flashMessage('Úprava se nezdařila: '. $str);
		}
      
    }
	//*********************************************************
	
	// Change name
	//******************************************************
	protected function createComponentChangeNameForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeNameFormSucceeded'];
        return $form;

    }
    
    public function changeNameFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['krestni_jmeno'] = $form->getHttpData($form::DATA_LINE, 'krestni_jmeno');
        $values['prijmeni'] = $form->getHttpData($form::DATA_LINE, 'prijmeni');

		try{
			$result = $this->database->table('hraci')->where("uziv_jmeno= ? ",$this->user->identity->uziv_jmeno)->update($values);
			$this->user->identity->krestni_jmeno = $values['krestni_jmeno'];
			$this->user->identity->prijmeni = $values['prijmeni'];
			$this->flashMessage("Byla provedena změna jména.");
			$this->redirect('User:Settings');
		} catch(Nette\Database\DriverException $exception){
			$str = $exception->getMessage();
			$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
			$this->flashMessage('Úprava se nezdařila: '. $str);
		}
      
    }
	//*********************************************************
	// Delete name
	//******************************************************
	protected function createComponentDeleteNameForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'deleteNameFormSucceeded'];
        return $form;

    }
    
    public function deleteNameFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['krestni_jmeno'] = "";
        $values['prijmeni'] = "";

		try{
			$result = $this->database->table('hraci')->where("uziv_jmeno= ? ",$this->user->identity->uziv_jmeno)->update($values);
			$this->user->identity->krestni_jmeno = $values['krestni_jmeno'];
			$this->user->identity->prijmeni = $values['prijmeni'];
			$this->flashMessage("Jméno bylo smazáno.");
			$this->redirect('User:Settings');
		} catch(Nette\Database\DriverException $exception){
			$str = $exception->getMessage();
			$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
			$this->flashMessage('Úprava se nezdařila: '. $str);
		}
      
    }
	//*********************************************************
	// Change Email
	//*********************************************************
	protected function createComponentChangeEmailForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeEmailFormSucceeded'];
        return $form;

    }
    
    public function changeEmailFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['email'] = $form->getHttpData($form::DATA_LINE, 'email');

		try{
			$result = $this->database->table('hraci')->where("uziv_jmeno= ? ",$this->user->identity->uziv_jmeno)->update($values);
			$this->user->identity->email = $values['email'];
			$this->flashMessage("Byla provedena změna emailu.");
			$this->redirect('User:Settings');
		} catch(Nette\Database\DriverException $exception){
			$str = $exception->getMessage();
			$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
			$this->flashMessage('Úprava se nezdařila: '. $str);
		}
      
    }
	//*********************************************************
	
	// Change Password
	//*********************************************************
	protected function createComponentChangePasswordForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changePasswordFormSucceeded'];
        return $form;

    }
    
    public function changePasswordFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['heslo'] = $form->getHttpData($form::DATA_LINE, 'heslo');
		$compare = $form->getHttpData($form::DATA_LINE, 'heslo2');

		if ($values['heslo'] == $compare) {
			try{
				$result = $this->database->table('hraci')->where("uziv_jmeno= ? ",$this->user->identity->uziv_jmeno)->update($values);
				$this->flashMessage("Byla provedena změna hesla.");
				$this->redirect('User:Settings');
			} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Úprava se nezdařila: '. $str);
			}
		}else {
			$this->flashMessage("Hesla se neshodují.");
		}
    	
      
    }
	//**********************************************************
	
	// Send request for dungeon master
	//*********************************************************
	protected function createComponentSendDMRequestForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'sendDMRequestFormSucceeded'];
        return $form;

    }
    
    public function sendDMRequestFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
	
		$value['ID_hrace'] = $this->user->identity->ID_hrace;
		try{
			$this->database->table("zadosti")->insert($value);
			$this->flashMessage("Žádost o povýšení na pána jeskyně byla odeslána, prosím vyčkejte na potvrzení administrátora.");
		} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Žádost se nezdařila: '. $str);
		}
    	
        $this->redirect('User:Settings');
      
    }
	//**********************************************************
	
}
