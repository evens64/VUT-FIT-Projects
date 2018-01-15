<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;

class GamePresenter extends BasePresenter
{

	private $database;

	public function __construct(Nette\Database\Context $database) {
		$this->database = $database;
	}

	public function renderDefault($id_sezeni)
	{
		if (!(isset($id_sezeni))) {
			$this->flashmessage("Není definováno sezení");
			$this->redirect("User:Default");
		}

		$this->template->sezeni =                $this->database->query("SELECT * FROM hra_sezeni WHERE ID_sezeni = ?", $id_sezeni)->fetch();
		if ($this->template->sezeni == NULL) {
			$this->flashmessage("Neexistující sezení");
			$this->redirect("User:Default");
		}

		$this->template->dobrodruzstvi =         $this->database->query("SELECT * FROM hra_dobrodruzstvi WHERE ID_dobrodruzstvi = (SELECT ID_dobrodruzstvi FROM hra_sezeni WHERE ID_sezeni = ?)", $id_sezeni)->fetch();

		if ($this->template->dobrodruzstvi['ID_tazeni'] != "") {
			$this->template->tazeni =            $this->database->query("SELECT * FROM hra_tazeni WHERE ID_tazeni = (SELECT ID_tazeni FROM hra_dobrodruzstvi WHERE ID_dobrodruzstvi = (SELECT ID_dobrodruzstvi FROM hra_sezeni WHERE ID_sezeni = ?))", $id_sezeni)->fetch();
		} else {
			$this->template->tazeni = NULL;
		}

		$this->template->ucastnici =             $this->database->query("SELECT * FROM postavy WHERE ID_postavy IN (SELECT ID_postavy FROM mn_ucastnici WHERE ID_sezeni = ?) ORDER BY jmeno", $id_sezeni);

		$this->template->radku = (int)($this->template->ucastnici->getRowCount() / 4);
		if ($this->template->ucastnici->getRowCount() % 4 > 0) $this->template->radku++;

	}

}
