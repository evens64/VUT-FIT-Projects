<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Nette\Database;

class DmPresenter extends BasePresenter
{

	private $database;

	public function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

    public function renderDefault(){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		
	// pripoj podle id jeste tabulky dobroduzstvi a tazeni a vsechno vykombit dohromady		
		$this->template->neukoncene_sezeni 			= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=0 AND DATE(hra_sezeni.kdy)>=CURDATE() ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);
																			
		$this->template->neukoncene_prosle_sezeni 	= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=0 AND DATE(hra_sezeni.kdy)<CURDATE() ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);
																			
		$this->template->spustene_sezeni 	= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=2 ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);																		
		
    }

	public function ActionUkoncitSezeni($id){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		
	    $values['priznak'] = 1;
		$rows = $this->database->table("hra_sezeni")->where("ID_hrace=? AND ID_sezeni=?",$this->user->identity->ID_hrace,$id)->update($values);
	
		if ($rows>0)
			$this->flashMessage("Právě jste ukončil sezení.");
		else
			$this->flashMessage("Nemůžete ukončit danné sezení.");
		$this->redirect("Dm:default");
	}
	
	public function ActionSpustitSezeni($id){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		
	    $values['priznak'] = 2;
		$rows = $this->database->table("hra_sezeni")->where("ID_hrace=? AND ID_sezeni=?",$this->user->identity->ID_hrace,$id)->update($values);
	
		if ($rows>0){
			$this->flashMessage("Právě jste spustil sezení.");
			$this->redirect("Game:default");
		}else {
			$this->flashMessage("Nemůžete spustit danné sezení.");
			$this->redirect("Dm:default");
		}
	}
	
	// Change dateAndTime
	//******************************************************
	protected function createComponentChangeDateAndTimeForm()
    {
        $form = new Form();
        $form->addProtection('Platnost formuláře vypršela! Zkuste to prosím znovu.');
        $form->onSuccess[] =  [$this, 'changeDateAndTimeFormSucceeded'];
        return $form;

    }
    
    public function changeDateAndTimeFormSucceeded(Nette\Application\UI\Form $form, $values)
    {
        $id = $form->getHttpData($form::DATA_LINE, 'ID_sezeni');
		
        $kde= $form->getHttpData($form::DATA_LINE, 'kde');
        $kdy = $form->getHttpData($form::DATA_LINE, 'kdy');
		if ($kde != "")
			$values['kde'] = $kde;
			
		if ($kdy != "")
			$values['kdy'] = $kdy;
		
    	$result = $this->database->table('hra_sezeni')->where("ID_sezeni=?",$id)->update($values);
		$this->flashMessage("Byla provedena změna.");
        $this->redirect('Dm:default');
      
    }
	//*********************************************************
	
    public function renderCampaigns(){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		
		$this->template->neukoncene_tazeni =	$this->database->query	
									("SELECT T.ID_tazeni ID_tazeni, T.cil cil,
										(SELECT COUNT(*) FROM hra_dobrodruzstvi D WHERE D.ID_tazeni = T.ID_tazeni ) pocet_dobrodruzstvi,
										(SELECT COUNT(*) FROM hra_sezeni S WHERE S.ID_dobrodruzstvi = D.ID_dobrodruzstvi) pocet_sezeni
										FROM hra_tazeni T, hra_dobrodruzstvi D
										WHERE D.ID_hrace = ? AND D.priznak = 0",$this->user->identity->ID_hrace);
									
		$this->template->ukoncene_tazeni =	$this->database->query	
									("SELECT T.ID_tazeni ID_tazeni, T.cil cil,
									(SELECT COUNT(*) FROM hra_dobrodruzstvi D WHERE D.ID_tazeni = T.ID_tazeni ) pocet_dobrodruzstvi,
									(SELECT COUNT(*) FROM hra_sezeni S WHERE S.ID_dobrodruzstvi = D.ID_dobrodruzstvi) pocet_sezeni
									FROM hra_tazeni T, hra_dobrodruzstvi D
									WHERE D.ID_hrace = ? AND D.priznak = 1",$this->user->identity->ID_hrace);
    }
	
	public function ActionUkoncitTazeni($id){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
	    $values['priznak'] = 1;
		$rows = $this->database->table("hra_tazeni")->where("ID_hrace=? AND ID_tazeni=?",$this->user->identity->ID_hrace,$id)->update($values);
	
		if ($rows>0)
			$this->flashMessage("Právě jste ukončil sezení.");
		else
			$this->flashMessage("Nemůžete ukončit danné sezení.");
		$this->redirect("Dm:Campaigns");
	}


    public function renderQuests(){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		$this->template->neukoncene_dobrodruzstvi =	$this->database->query	
									(" SELECT T.cil cil_tazeni, D.cil cil_dobrodruzstvi, D.ID_dobrodruzstvi ID_dobrodruzstvi, 
									(SELECT COUNT(*) FROM hra_sezeni S WHERE S.ID_dobrodruzstvi = D.ID_dobrodruzstvi AND D.ID_tazeni = T.ID_tazeni) pocet_sezeni
									FROM hra_tazeni T, hra_dobrodruzstvi D
									WHERE D.ID_hrace = ? AND D.priznak = 0",$this->user->identity->ID_hrace);
								
		$this->template->ukoncene_dobrodruzstvi =	$this->database->query	
									(" SELECT T.cil cil_tazeni, D.cil cil_dobrodruzstvi,
									(SELECT COUNT(*) FROM hra_sezeni S WHERE S.ID_dobrodruzstvi = D.ID_dobrodruzstvi AND D.ID_tazeni = T.ID_tazeni) pocet_sezeni
									FROM hra_tazeni T, hra_dobrodruzstvi D
									WHERE D.ID_hrace = ? AND D.priznak = 1",$this->user->identity->ID_hrace);
									
    }
	
	public function ActionUkoncitDobrodruzstvi($id){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
	    $values['priznak'] = 1;
		$rows = $this->database->table("hra_dobrodruzstvi")->where("ID_hrace=? AND ID_dobrodruzstvi=?",$this->user->identity->ID_hrace,$id)->update($values);
	
		if ($rows>0)
			$this->flashMessage("Právě jste ukončil dobrodružství.");
		else
			$this->flashMessage("Nemůžete ukončit danné dobrodružství.");
		$this->redirect("Dm:Quests");
	}


    public function renderSittings(){
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
				$this->redirect('Homepage:Default');
		}
		if (!($this->user->identity->pan_jeskyne)){
			$this->flashMessage("Pro zobrazení této stránky musíte být Pánem jeskyně.");
			$this->redirect('Homepage:Default');
		}
		$this->template->neukoncene_sezeni 			= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=0 AND DATE(hra_sezeni.kdy)>=CURDATE() ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);
																			
		$this->template->ukoncene_sezeni 			= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=1 ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);
																			
		$this->template->neukoncene_prosle_sezeni 	= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=0 AND DATE(hra_sezeni.kdy)<CURDATE() ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);
																			
		$this->template->spustene_sezeni 			= $this->database->query("SELECT hra_sezeni.ID_sezeni, hra_sezeni.kdy, hra_sezeni.kde, hra_tazeni.cil AS tazeni, hra_dobrodruzstvi.cil AS dobrodruzstvi 
																			FROM hra_sezeni 
																			INNER JOIN hra_dobrodruzstvi ON hra_sezeni.ID_dobrodruzstvi=hra_dobrodruzstvi.ID_dobrodruzstvi 
																			INNER JOIN hra_tazeni ON hra_tazeni.ID_tazeni=hra_dobrodruzstvi.ID_tazeni 
																			WHERE hra_sezeni.ID_hrace=? AND hra_sezeni.priznak=2 ORDER BY DATE(hra_sezeni.kdy) ASC",
																			$this->user->identity->ID_hrace);	
    }


}
