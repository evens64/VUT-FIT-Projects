<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;



class AdminPresenter extends BasePresenter
{

	private $database;

	public function __construct(Nette\Database\Context $database) {
		$this->database = $database;
	}

	public function renderDefault() {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->zadosti = $this->database->query("SELECT ID_hrace, krestni_jmeno, prijmeni, prezdivka, ID_zadosti FROM hraci NATURAL JOIN zadosti WHERE priznak = 0");
	}
	
	public function ActionSchvalitZadost($id_zadosti,$id_hrace){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$zadosti["priznak"] = 1;		
		$hraci["pan_jeskyne"] = 1;
		$this->database->table("zadosti")->where("ID_zadosti=?",$id_zadosti)->update($zadosti);
		$this->database->table("hraci")->where("ID_hrace=?",$id_hrace)->update($hraci);
		$this->flashMessage("Žádost byla schválena.");
		$this->redirect('Admin:Default');
	}
	
	public function ActionZamitnoutZadost($id_zadosti){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$zadosti["priznak"] = 2;
		$this->database->table("zadosti")->where("ID_zadosti=?",$id_zadosti)->update($zadosti);
		$this->flashMessage("Žádost byla zamítnuta.");
		$this->redirect('Admin:Default');
	}
	

	public function renderUsers() {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->hraci = $this->database->query("SELECT * FROM hraci");
	}
	
	
	// Edit user
	//******************************************************
	protected function createComponentEditUserForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'editUserFormSucceeded'];
        return $form;

    }
    
    // TODO 
    public function editUserFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['uziv_jmeno'] = $form->getHttpData($form::DATA_LINE, 'uziv_jmeno');
        $values['prezdivka'] = $form->getHttpData($form::DATA_LINE, 'prezdivka');
        $values['krestni_jmeno'] = $form->getHttpData($form::DATA_LINE, 'krestni_jmeno');
        $values['prijmeni'] = $form->getHttpData($form::DATA_LINE, 'prijmeni');
        $values['email'] = $form->getHttpData($form::DATA_LINE, 'email');
        $values['heslo'] = $form->getHttpData($form::DATA_LINE, 'heslo');
        $values['pohlavi'] = $form->getHttpData($form::DATA_LINE, 'pohlavi');
        $id = $form->getHttpData($form::DATA_LINE, 'ID_hrace');
		

    	$result = $this->database->table('hraci')->where("ID_hrace=?",$id)->update($values);
		$this->flashMessage("Hráč byl upraven.");
        $this->redirect('Admin:Users');
      
    }
	//*********************************************************
		
	public function ActionSmazatHrace($id_hrace){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("hraci")->where("ID_hrace=?",$id_hrace)->delete();
		$this->flashMessage("Hráč byl smazán.");
		$this->redirect('Admin:Users');
	}
	
	//***********************************************************************************
	//***********************************************************************************

	public function renderCharacters() {
		
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->postavy = $this->database->query("SELECT * FROM postavy WHERE ID_postavy NOT IN (SELECT ID_postavy FROM mn_umrti)");
	}
	
	// Edit char
	//******************************************************
	protected function createComponentChangeCharForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeCharFormSucceeded'];
        return $form;

    }
    
     
    public function changeCharFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['jmeno'] = $form->getHttpData($form::DATA_LINE, 'jmeno');
        $values['pohlavi'] = $form->getHttpData($form::DATA_LINE, 'pohlavi');
        $values['rasa'] = $form->getHttpData($form::DATA_LINE, 'rasa');
        $values['povolani'] = $form->getHttpData($form::DATA_LINE, 'povolani');
        $values['stat_XP'] = $form->getHttpData($form::DATA_LINE, 'stat_XP');
        $values['stat_MN'] = $form->getHttpData($form::DATA_LINE, 'stat_MN');
        $values['stat_HP'] = $form->getHttpData($form::DATA_LINE, 'stat_HP');
        $values['stat_SP'] = $form->getHttpData($form::DATA_LINE, 'stat_SP');
        $values['stat_uroven'] = $form->getHttpData($form::DATA_LINE, 'stat_uroven');
        $id = $form->getHttpData($form::DATA_LINE, 'ID_postavy');
		

    	$result = $this->database->table('postavy')->where("ID_postavy=?",$id)->update($values);
		$this->flashMessage("Postava byla upravena.");
        $this->redirect('Admin:Characters');
      
    }
	//*********************************************************
	public function ActionSmazatPostavu($id_postavy){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("postavy")->where("ID_postavy=?",$id_postavy)->delete();
		$this->flashMessage("Postava byla smazána.");
		$this->redirect('Admin:Characters');
	}
	
	public function ActionVyprazdnitInventar($id_postavy){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("mn_inventare")->where("ID_postavy=?",$id_postavy)->delete();
		$this->flashMessage("Inventář byl vyprázdněn.");
		$this->redirect('Admin:Characters');
	}
	
	public function ActionOzivitPostavu($id_postavy,$uroven){
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$values['stat_HP'] = 100* $uroven;
		$values['stat_MN'] = 100* $uroven;
		$this->database->table("postavy")->where("ID_postavy=?",$id_postavy)->update($values);
		$this->database->table("mn_umrti")->where("ID_postavy=?",$id_postavy)->delete();
		$this->flashMessage("Postava byla oživena.");
		$this->redirect('Admin:Characters');
	}
	
	//************************************************************************************

	public function renderCharactersdead() {
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->postavy = $this->database->query("SELECT * FROM postavy WHERE ID_postavy IN (SELECT ID_postavy FROM mn_umrti)");
	}
	
	//*************************************************************************************

	public function renderCampaigns() {
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->tazeni = $this->database->query("SELECT * FROM hra_tazeni");
	}
	
	// Edit tazeni
	//******************************************************
	protected function createComponentChangeCampaignForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeCampaignFormSucceeded'];
        return $form;

    }
    
     
    public function changeCampaignFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['cil'] = $form->getHttpData($form::DATA_LINE, 'cil');
        $values['mapa'] = $form->getHttpData($form::DATA_LINE, 'mapa');
        $values['priznak'] = $form->getHttpData($form::DATA_LINE, 'priznak');
        $id = $form->getHttpData($form::DATA_LINE, 'ID_tazeni');
		

    		$result = $this->database->table('hra_tazeni')->where("ID_tazeni=?",$id)->update($values);
			$this->flashMessage("Tažení bylo upraveno.");
       	$this->redirect('Admin:Campaigns');
      
    }
	
	//************************************************************************************
	public function ActionSmazatTazeni($id_tazeni){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("hra_tazeni")->where("ID_tazeni=?",$id_tazeni)->delete();
		$this->flashMessage("Tažení bylo smazáno.");
		$this->redirect('Admin:Campaigns');
	}

	//***********************************************************************************
	public function renderQuests() {
		
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->dobrodruzstvi = $this->database->query("SELECT * FROM hra_dobrodruzstvi");
	}
	
	
	// Edit dobrodruzstvi
	//******************************************************
	protected function createComponentChangeQuestsForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeQuestsFormSucceeded'];
        return $form;

    }
    
     
    public function changeQuestsFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['cil'] = $form->getHttpData($form::DATA_LINE, 'cil');
        $values['obtiznost'] = $form->getHttpData($form::DATA_LINE, 'obtiznost');
        $values['lokalita'] = $form->getHttpData($form::DATA_LINE, 'lokalita');
        $values['priznak'] = $form->getHttpData($form::DATA_LINE, 'priznak');
        $id = $form->getHttpData($form::DATA_LINE, 'ID_dobrodruzstvi');
		

    		$result = $this->database->table('hra_dobrodruzstvi')->where("ID_dobrodruzstvi=?",$id)->update($values);
			$this->flashMessage("Dobrodružství bylo upraveno.");
       	$this->redirect('Admin:Quests');
      
    }
    
	//************************************************************************************
	public function ActionSmazatDobrodruzstvi($id_dobrodruzstvi){
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("hra_dobrodruzstvi")->where("ID_dobrodruzstvi=?",$id_dobrodruzstvi)->delete();
		$this->flashMessage("Dobrodružství bylo smazáno.");
		$this->redirect('Admin:Quests');
	}
	
	
	//************************************************************************************

	public function renderSittings() {
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->sezeni = $this->database->query("SELECT * FROM hra_sezeni");
	}
	
	// Edit sezení
	//******************************************************
	protected function createComponentChangeSittingsForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeSittingsFormSucceeded'];
        return $form;

    }
    
     
    public function changeSittingsFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['kde'] = $form->getHttpData($form::DATA_LINE, 'kde');
        $values['priznak'] = $form->getHttpData($form::DATA_LINE, 'priznak');
        $id = $form->getHttpData($form::DATA_LINE, 'ID_sezeni');
		

    		$result = $this->database->table('hra_sezeni')->where("ID_sezeni=?",$id)->update($values);
			$this->flashMessage("Sezení bylo upraveno.");
       	$this->redirect('Admin:Sittings');
      
    }
    //********************************************************************************
    
    public function ActionSmazatSezeni($id_sezeni){
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("hra_sezeni")->where("ID_sezeni=?",$id_sezeni)->delete();
		$this->flashMessage("Sezení bylo smazáno.");
		$this->redirect('Admin:Sittings');
	}
	
	//***********************************************************************************

	public function renderItems() {
	
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}

		$this->template->predmety = $this->database->query("SELECT * FROM predmety");
	}
	
	// Edit items
	//******************************************************
	protected function createComponentChangeItemsForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeItemsFormSucceeded'];
        return $form;

    }
    
     
    public function changeItemsFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $values['nazev'] = $form->getHttpData($form::DATA_LINE, 'nazev');
        $values['popis'] = $form->getHttpData($form::DATA_LINE, 'popis');
        $values['kvalita'] = $form->getHttpData($form::DATA_LINE, 'kvalita');
        $values['priznak'] = $form->getHttpData($form::DATA_LINE, 'priznak');
        $id = $form->getHttpData($form::DATA_LINE, 'ID_predmetu');
		

    		$result = $this->database->table('predmety')->where("ID_predmetu=?",$id)->update($values);
			$this->flashMessage("Předmět byl upraven.");
       	$this->redirect('Admin:Items');
      
    }
    //********************************************************************************
     public function ActionSmazatPredmet($id_predmetu){
	 
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->admin)) {
				$this->flashMessage("Nemáte práva vidět dannou stránku.");
				$this->redirect('Homepage:Default');
		}
		
		$this->database->table("predmety")->where("ID_predmetu=?",$id_predmetu)->delete();
		$this->flashMessage("Předmět byl smazán.");
		$this->redirect('Admin:Items');
	}
	

}
