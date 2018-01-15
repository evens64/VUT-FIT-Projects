<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Nette\Security\User;
use Nette\Http\Session;

class LoginPresenter extends BasePresenter
{
	 private $user;
	 private $database;
	 private $accounts;
	 private $passwords;
	 private $session;
	 private $sessionSection;
	
	public function __construct(Nette\Security\User $user,Nette\Database\Context $database, Nette\Http\Session $session)	{
		
	  $this->database = $database;
      $this->user = $user;
      $this->session = $session->getSection("mine");
      
		$this->accounts = $this->database->table('hraci');
		
		foreach ($this->accounts as $acc){
			$final[$acc['uziv_jmeno']] = $acc['heslo'];		
		}
		
		$authenticator = new Nette\Security\SimpleAuthenticator($final);
		$this->user->setAuthenticator($authenticator);		      
      
	}
	
	public function renderDefault(){
     if ($this->user->isLoggedIn()) {
        		$this->redirect('User:Default');
    	}
	}
	
	public function actionOut(){
		$this->user->logout();
    	$this->flashMessage('Odhlášení bylo úspěšné.');
    	$this->redirect("Homepage:Default");
	}
	
    protected function createComponentLoginForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
		$form->addSubmit('login','Přihlásit')->setAttribute("class","btn btn-lg btn-outline-light btn-block")->setAttribute("style","background-color: #2d0000");
        $form->onSuccess[] =  [$this, 'loginFormSucceeded'];
        return $form;

    }
    
    public function loginFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $username = $form->getHttpData($form::DATA_TEXT, 'userName');
        $password = $form->getHttpData($form::DATA_TEXT, 'password');
        

        try {
    	  	$this->user->login($username, $password);
    	  	$acc = $this->database->table('hraci')->where("uziv_jmeno= ? ",$username);		 
		  
		  	foreach($acc as $ac){
						$this->user->identity->ID_hrace 		= $ac['ID_hrace'];
						$this->user->identity->uziv_jmeno 		= $ac['uziv_jmeno'];
						$this->user->identity->email 			= $ac['email'];
						$this->user->identity->prezdivka 		= $ac['prezdivka'];
						$this->user->identity->krestni_jmeno 	= $ac['krestni_jmeno'];
						$this->user->identity->prijmeni 		= $ac['prijmeni'];
						$this->user->identity->datum_narozeni 	= $ac['datum_narozeni'];
						$this->user->identity->pan_jeskyne 		= $ac['pan_jeskyne'];
						$this->user->identity->admin 			= $ac['admin'];
		  	}    	  
		  	
        	$this->redirect('User:Default');
		  	$this->user->setExpiration('30 minutes', TRUE);

    	} catch (Nette\Security\AuthenticationException $e) {
      	$this->flashMessage('Nesprávné přihlašovací jméno nebo heslo.');
    	}
      
    }

}
