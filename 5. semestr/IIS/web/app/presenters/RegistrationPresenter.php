<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Nette\Database;


class RegistrationPresenter extends BasePresenter
{

	private $database;
	
	public function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

    public function renderDefault()
    {
        if ($this->user->isLoggedIn()) {
        		$this->redirect('User:Default');
    	}
    }
	
	protected function createComponentRegistrationForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'registrationFormSucceeded'];
        return $form;

    }
	
	public function registrationFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
		$heslo = $form->getHttpData($form::DATA_LINE, 'heslo');
		$heslo2 = $form->getHttpData($form::DATA_LINE, 'heslo2');
		if ($heslo === $heslo2) {
	
			$values['uziv_jmeno'] 		= $form->getHttpData($form::DATA_LINE, 'uziv_jmeno');
			$values['heslo'] 			= $heslo;
			$values['email'] 			= $form->getHttpData($form::DATA_LINE, 'email');
			$values['krestni_jmeno'] 	= $form->getHttpData($form::DATA_LINE, 'krestni_jmeno');
			$values['prijmeni'] 		= $form->getHttpData($form::DATA_LINE, 'prijmeni');
			$values['prezdivka'] 		= $form->getHttpData($form::DATA_LINE, 'prezdivka');
			$values['datum_narozeni'] 	= $form->getHttpData($form::DATA_LINE, 'datum_narozeni');
			$values['pohlavi'] 			= $form->getHttpData($form::DATA_LINE, 'pohlavi');
			$values['admin'] 			= 0;
			$values['pan_jeskyne'] 		= 0;
        
			try{
				$acc = $this->database->table('hraci')->insert($values); 
		
				$this->flashMessage('Registrace proběhla úspěšně! Nyní se můžete přihlásit.');
				$this->redirect('Login:Default');
			} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Registrace se nezdařila: '. $str);
			}
			
		} else {
			$this->flashMessage('Registrace se nezdařila, zadaná hesla se neshodují.');
		}
      
    }

}
