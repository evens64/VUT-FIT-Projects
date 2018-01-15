<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Nette\Database;

class CreatePresenter extends BasePresenter
{
	private $database;

	public function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	/* redirect na homepage */
	public function renderDefault() {
		$this->redirect("Homepage:Default");
	}

	/* vytvoř postavu */
	public function renderCharacter() {
		if (!($this->user->isLoggedIn())) {
			$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
			$this->redirect('Homepage:Default');
		}
	}

	// Create character
	//******************************************************
	protected function createComponentCharacterForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'characterFormSucceeded'];
        return $form;

    }
    
    public function characterFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['ID_hrace'] = $this->user->identity->ID_hrace;
        $values['jmeno'] = $form->getHttpData($form::DATA_LINE, 'jmeno');
        $values['rasa'] = $form->getHttpData($form::DATA_LINE, 'rasa');
        $values['pohlavi'] = $form->getHttpData($form::DATA_LINE, 'pohlavi');
        $values['povolani'] = $form->getHttpData($form::DATA_LINE, 'povolani');

		try{
			$result = $this->database->table('postavy')->insert($values);
			$this->flashMessage("Postava byla vytvořena.");
			$this->redirect('User:Default');
		} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Přidání se nezdařilo: '. $str);
		}
      
    }
	//*********************************************************

	/* vytvoř tažení */
	public function renderCampaign() {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
	}
	
	// Create tazeni / campaign
	//******************************************************
	protected function createComponentCampaignForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'campaignFormSucceeded'];
        return $form;

    }
    
    public function campaignFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['cil'] = $form->getHttpData($form::DATA_LINE, 'cil');
        $values['mapa'] = $form->getHttpData($form::DATA_LINE, 'mapa');
        $values['ID_hrace'] = $this->user->identity->ID_hrace;

		try{
			$result = $this->database->table('hra_tazeni')->insert($values);
			$this->flashMessage("Tažení bylo vytvořeno.");
			$this->redirect('Dm:Campaigns');
		} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Přidání se nezdařilo: '. $str);
		}
		
    }
	//*********************************************************

	/* vytvoř dobrodružství */
	public function renderQuest() {
		if (!($this->user->isLoggedIn())) {
			$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
			$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		$this->template->tazeni = $this->database->table("hra_tazeni")->where("ID_hrace=? AND priznak=0",$this->user->identity->ID_hrace);
	}

	// Create dobrodružství / quest
	//******************************************************
	protected function createComponentQuestForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'questFormSucceeded'];
        return $form;

    }
    
    public function questFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['cil'] = $form->getHttpData($form::DATA_LINE, 'cil');
        $values['lokalita'] = $form->getHttpData($form::DATA_LINE, 'lokalita');
        $idTazeni = $form->getHttpData($form::DATA_LINE, 'ID_tazeni');
        $values['obtiznost'] = $form->getHttpData($form::DATA_LINE, 'obtiznost');
        $values['ID_hrace'] = $this->user->identity->ID_hrace;
		
		if ($idTazeni != "")
			$values['ID_tazeni'] = $idTazeni ;

		try{
			$result = $this->database->table('hra_dobrodruzstvi')->insert($values);
			$this->flashMessage("Dobrodružství bylo vytvořeno.");
			$this->redirect('Dm:Quests');
		} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Přidání se nezdařilo: '. $str);
		}
      
    }
	//*********************************************************
	
	/* vytvoř sezení */
	public function renderSitting() {
		if (!($this->user->isLoggedIn())) {
			$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
			$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		$this->template->dobrodruzstvi = $this->database->table("hra_dobrodruzstvi")->where("priznak=0");
	}
	
	// Create sezeni / sitting
	//******************************************************
	protected function createComponentSittingForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'sittingFormSucceeded'];
        return $form;

    }
    
    public function sittingFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
	
        $values['kde'] = $form->getHttpData($form::DATA_LINE, 'kde');
        $values['ID_dobrodruzstvi'] = $form->getHttpData($form::DATA_LINE, 'ID_dobrodruzstvi');
        $values['kdy'] = $form->getHttpData($form::DATA_LINE, 'kdy');
        $values['ID_hrace'] = $this->user->identity->ID_hrace;

		try{
			$result = $this->database->table('hra_sezeni')->insert($values);
			$this->flashMessage("Sezeni bylo vytvořeno.");
			$this->redirect('Dm:Sittings');
		} catch(Nette\Database\DriverException $exception){
				$str = $exception->getMessage();
				$str = str_replace("SQLSTATE[45000]: <<Unknown error>>:","",$str);
				$this->flashMessage('Přidání se nezdařilo: '. $str);
		}
		
      
    }
	//*********************************************************

}
