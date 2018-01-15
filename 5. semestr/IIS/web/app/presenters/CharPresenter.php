<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;



class CharPresenter extends BasePresenter
{

	private $database;
	
	public function __construct(Nette\Database\Context $database) {
		$this->database = $database;
	}

	public function renderDefault($char) {
		if (!($this->user->isLoggedIn())) {
				$this->flashMessage("Pro zobrazení této stránky musíte být přihlášení.");
        		$this->redirect('Homepage:Default');
    	}
		
		$this->template->char = $this->database->table("postavy")->where("ID_hrace=? AND ID_postavy=?",$this->user->identity->ID_hrace,$char);
		$this->template->inventory = $this->database->query("
		SELECT predmety.nazev,hra_sezeni.kde,hra_sezeni.kdy,predmety.popis,predmety.kvalita,predmety.priznak FROM mn_inventare 
		JOIN predmety ON predmety.ID_predmetu=mn_inventare.ID_predmetu 
		JOIN hra_sezeni ON hra_sezeni.ID_sezeni=mn_inventare.ID_sezeni
		WHERE mn_inventare.ID_postavy=?
		",$char);
	}

}
