<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;

class HomepagePresenter extends BasePresenter
{
	private $database;

	public function __construct(Nette\Database\Context $database){
		parent::__construct();
		$this->database = $database;
	}

    public function renderDefault(){
		if ($this->user->isLoggedIn()) {
			$this->redirect('Homepage:Sittings');
		}
    }

	public function renderSittings(){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
        		$this->redirect('Homepage:Default');
    	}

		$this->template->moje_postavy = $this->database->query("SELECT * FROM postavy 
													WHERE ID_hrace=? AND ID_postavy 
													NOT IN (SELECT ID_postavy FROM mn_ucastnici);
													",$this->user->identity->ID_hrace)->fetchall();

		$this->template->sezeni = $this->database->query("
						SELECT * FROM hra_sezeni
						WHERE hra_sezeni.priznak=0 ORDER BY kdy ASC");

    }
    
    
    // Prihlasit na sezeni
	//******************************************************
	protected function createComponentPrihlasitPostavuForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'prihlasitPostavuFormSucceeded'];
        return $form;

    }
    
    public function prihlasitPostavuFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['ID_postavy'] = $form->getHttpData($form::DATA_LINE, 'ID_postavy');
        $values['ID_sezeni'] = $form->getHttpData($form::DATA_LINE, 'ID_sezeni');

    	$result = $this->database->table('mn_ucastnici')->insert($values);
		$this->flashMessage("Postava byla přihlášená.");
        $this->redirect('Homepage:Sittings');
      
    }
	//*********************************************************
	
	
	
}
