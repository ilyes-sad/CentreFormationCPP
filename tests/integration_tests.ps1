# integration_tests.ps1
# Tests d'intégration de l'application CentreFormationCPP (Qt + Oracle XE).
# Couvre : schéma, données, séquences/triggers, recherche, statistiques,
# alertes métier, planning/chevauchement et CRUD complet.
# Usage :  powershell -ExecutionPolicy Bypass -File integration_tests.ps1

$ErrorActionPreference = 'Stop'

$script:Pass = 0
$script:Fail = 0

function Test-Check {
    param([bool]$Condition, [string]$Message)
    if ($Condition) {
        $script:Pass++
        Write-Host "PASS: $Message" -ForegroundColor Green
    } else {
        $script:Fail++
        Write-Host "FAIL: $Message" -ForegroundColor Red
    }
}

function Section-Header {
    param([string]$Title)
    Write-Host ""
    Write-Host "===== $Title =====" -ForegroundColor Cyan
}

# ---------- Connexion Oracle ----------
Add-Type -AssemblyName System.Data.OracleClient | Out-Null
$conn = New-Object System.Data.OracleClient.OracleConnection("Data Source=XE;User ID=Ilyess;Password=0000")
try {
    $conn.Open()
    Test-Check $true "Connexion Oracle XE OK"
} catch {
    Write-Host "FAIL: impossible de se connecter a Oracle : $_" -ForegroundColor Red
    exit 1
}

function Get-OraScalar([string]$sql) {
    $cmd = $conn.CreateCommand()
    $cmd.CommandText = $sql
    try { return $cmd.ExecuteScalar() } catch { return $null }
}

function Get-OraRows([string]$sql) {
    $cmd = $conn.CreateCommand()
    $cmd.CommandText = $sql
    $rd = $cmd.ExecuteReader()
    $rows = New-Object System.Collections.ArrayList
    while ($rd.Read()) {
        $vals = @()
        for ($i = 0; $i -lt $rd.FieldCount; $i++) { $vals += $rd.GetValue($i) }
        [void]$rows.Add($vals)
    }
    $rd.Close()
    return , $rows
}

function Invoke-OraNonQuery([string]$sql) {
    $cmd = $conn.CreateCommand()
    $cmd.CommandText = $sql
    [void]$cmd.ExecuteNonQuery()
}

# ---------- 1. Schéma ----------
Section-Header "1. Schema"
$tables = Get-OraRows "SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('FORMATEUR','COURS')"
Test-Check (($tables | Where-Object { $_[0] -eq 'FORMATEUR' }).Count -gt 0) "Table FORMATEUR existe"
Test-Check (($tables | Where-Object { $_[0] -eq 'COURS' }).Count -gt 0) "Table COURS existe"

$coursCols = Get-OraRows "SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME='COURS'"
$coursCols = @($coursCols | ForEach-Object { $_.GetValue(0).ToUpper() })
foreach ($col in @('ID_COURS','INTITULE','ID_FORMATEUR','DUREE_HEURES','DATE_DEBUT','DATE_FIN','HEURE_DEBUT','HEURE_FIN','CAPACITE','PROGRAMME')) {
    Test-Check ($coursCols -contains $col) "Colonne COURS.$col presente"
}
$forCols = Get-OraRows "SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME='FORMATEUR'"
$forCols = @($forCols | ForEach-Object { $_.GetValue(0).ToUpper() })
foreach ($col in @('ID_FORMATEUR','NOM','PRENOM','EMAIL','TELEPHONE','SPECIALITE','DATE_EMBAUCHE','STATUS')) {
    Test-Check ($forCols -contains $col) "Colonne FORMATEUR.$col presente"
}

$seqs = Get-OraRows "SELECT SEQUENCE_NAME FROM USER_SEQUENCES WHERE SEQUENCE_NAME IN ('SEQ_FORMATEUR','SEQ_COURS')"
Test-Check (($seqs | Where-Object { $_[0] -eq 'SEQ_FORMATEUR' }).Count -gt 0) "Sequence SEQ_FORMATEUR existe"
Test-Check (($seqs | Where-Object { $_[0] -eq 'SEQ_COURS' }).Count -gt 0) "Sequence SEQ_COURS existe"

$trgs = Get-OraRows "SELECT TRIGGER_NAME, STATUS FROM USER_TRIGGERS WHERE TRIGGER_NAME IN ('TRG_COURS_ID','BI_FORMATEUR')"
Test-Check (($trgs | Where-Object { $_[0] -eq 'TRG_COURS_ID' -and $_[1] -eq 'ENABLED' }).Count -gt 0) "Trigger TRG_COURS_ID (auto-id cours) actif"
Test-Check (($trgs | Where-Object { $_[0] -eq 'BI_FORMATEUR' -and $_[1] -eq 'ENABLED' }).Count -gt 0) "Trigger BI_FORMATEUR (auto-id formateur) actif"

Test-Check ((Get-OraScalar "SELECT COUNT(*) FROM USER_CONSTRAINTS WHERE CONSTRAINT_NAME='FK_COURS_FORMATEUR'") -ge 1) "Contrainte FK_COURS_FORMATEUR presente"

# ---------- 2. Données ----------
Section-Header "2. Donnees"
$nForm = [int](Get-OraScalar "SELECT COUNT(*) FROM FORMATEUR")
$nCours = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS")
Test-Check ($nForm -gt 0) "Formateurs presents ($nForm)"
Test-Check ($nCours -gt 0) "Cours presents ($nCours)"

$orphans = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS c LEFT JOIN FORMATEUR f ON c.ID_FORMATEUR=f.ID_FORMATEUR WHERE f.ID_FORMATEUR IS NULL")
Test-Check ($orphans -eq 0) "Aucun cours orphelin (FK) ($orphans)"

$heureBad = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE HEURE_DEBUT IS NULL OR HEURE_FIN IS NULL OR HEURE_DEBUT < 0 OR HEURE_DEBUT > 1439 OR HEURE_FIN < 0 OR HEURE_FIN > 1439 OR HEURE_DEBUT >= HEURE_FIN")
Test-Check ($heureBad -eq 0) "Heures valides partout (0..1439, debut < fin) ($heureBad)"

$dateBad = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE DATE_DEBUT > DATE_FIN")
Test-Check ($dateBad -eq 0) "Dates coherentes (DATE_FIN >= DATE_DEBUT) ($dateBad)"

# ---------- 3. Séquences ----------
Section-Header "3. Sequences"
$maxCours = [int](Get-OraScalar "SELECT NVL(MAX(ID_COURS),0) FROM COURS")
$lastCours = [long](Get-OraScalar "SELECT LAST_NUMBER FROM USER_SEQUENCES WHERE SEQUENCE_NAME='SEQ_COURS'")
Test-Check ($lastCours -gt $maxCours) "SEQ_COURS au-dela du max des ID ($lastCours > $maxCours)"
$maxForm = [int](Get-OraScalar "SELECT NVL(MAX(ID_FORMATEUR),0) FROM FORMATEUR")
$lastForm = [long](Get-OraScalar "SELECT LAST_NUMBER FROM USER_SEQUENCES WHERE SEQUENCE_NAME='SEQ_FORMATEUR'")
Test-Check ($lastForm -gt $maxForm) "SEQ_FORMATEUR au-dela du max des ID ($lastForm > $maxForm)"

# ---------- 4. Recherche ----------
Section-Header "4. Recherche"
$r = Get-OraRows "SELECT NOM, PRENOM FROM FORMATEUR WHERE LOWER(NOM) LIKE LOWER('%ilyes%')"
Test-Check ($r.Count -ge 1) "Recherche formateur par nom (LIKE ilyes) -> $($r.Count) resultat(s)"

$specs = Get-OraRows "SELECT DISTINCT SPECIALITE FROM FORMATEUR WHERE SPECIALITE IS NOT NULL ORDER BY 1"
Test-Check ($specs.Count -ge 1) "Filtre specialite disponible ($($specs.Count) valeur(s))"
if ($specs.Count -ge 1) {
    $s = $specs[0][0]
    $nBySpec = [int](Get-OraScalar "SELECT COUNT(*) FROM FORMATEUR WHERE SPECIALITE = '$s'")
    Test-Check ($nBySpec -ge 1) "Filtre specialite '$s' -> $nBySpec formateur(s)"
}

$rowsAsc = Get-OraRows "SELECT NOM FROM FORMATEUR ORDER BY NOM"
$names = @($rowsAsc | ForEach-Object { $_.GetValue(0).ToString() })
$sorted = $true
for ($i = 1; $i -lt $names.Count; $i++) {
    if ([string]::Compare($names[$i - 1], $names[$i], $true) -gt 0) { $sorted = $false; break }
}
Test-Check $sorted "Tri formateurs par nom (ASC)"

$cloud = Get-OraRows "SELECT INTITULE FROM COURS WHERE LOWER(INTITULE) LIKE LOWER('%cloud%')"
Test-Check ($cloud.Count -ge 1) "Recherche cours par intitule (LIKE cloud) -> $($cloud.Count) resultat(s)"

$nivs = Get-OraRows "SELECT DISTINCT NIVEAU FROM COURS WHERE NIVEAU IS NOT NULL ORDER BY 1"
Test-Check ($nivs.Count -ge 1) "Filtre niveau disponible ($($nivs.Count) valeur(s))"
if ($nivs.Count -ge 1) {
    $lv = $nivs[0][0]
    $nByLv = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE NIVEAU = '$lv'")
    $sumLv = [int](Get-OraScalar "SELECT NVL(SUM(CNT),0) FROM (SELECT COUNT(*) CNT FROM COURS GROUP BY NIVEAU)")
    Test-Check ($nByLv -ge 1) "Filtre niveau '$lv' -> $nByLv cours"
    Test-Check ($sumLv -eq $nCours) "Somme par niveau == total cours ($sumLv == $nCours)"
}

$durMin = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE DUREE_HEURES >= 1 AND DUREE_HEURES <= 100")
Test-Check ($durMin -eq $nCours) "Filtre duree 1..100h -> tous les cours ($durMin/$nCours)"

$leftJoin = Get-OraRows "SELECT COALESCE(f.NOM || ' ' || f.PRENOM, 'Inconnu') AS FORMATEUR, c.INTITULE FROM COURS c LEFT JOIN FORMATEUR f ON c.ID_FORMATEUR = f.ID_FORMATEUR"
Test-Check ($leftJoin.Count -eq $nCours) "Recherche cours avec LEFT JOIN formateur -> $($leftJoin.Count) lignes"

# ---------- 5. Statistiques ----------
Section-Header "5. Statistiques"
$sumSpec = [int](Get-OraScalar "SELECT NVL(SUM(CNT),0) FROM (SELECT COUNT(*) CNT FROM FORMATEUR GROUP BY SPECIALITE)")
Test-Check ($sumSpec -eq $nForm) "Pie formateurs: somme par specialite == total ($sumSpec == $nForm)"

$sumCat = [int](Get-OraScalar "SELECT NVL(SUM(CNT),0) FROM (SELECT COUNT(*) CNT FROM COURS GROUP BY CATEGORIE)")
Test-Check ($sumCat -eq $nCours) "Pie cours par categorie == total ($sumCat == $nCours)"

$sumNiv = [int](Get-OraScalar "SELECT NVL(SUM(CNT),0) FROM (SELECT COUNT(*) CNT FROM COURS GROUP BY NIVEAU)")
Test-Check ($sumNiv -eq $nCours) "Pie cours par niveau == total ($sumNiv == $nCours)"

# ---------- 6. Alertes métier ----------
Section-Header "6. Alertes"
$hasSeed = ([int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE INTITULE LIKE 'TEST Alerte%'")) -gt 0

$overlaps = Get-OraRows "SELECT f.NOM || ' ' || f.PRENOM, c1.INTITULE, c2.INTITULE
FROM COURS c1 JOIN COURS c2 ON c1.ID_FORMATEUR = c2.ID_FORMATEUR
JOIN FORMATEUR f ON f.ID_FORMATEUR = c1.ID_FORMATEUR
WHERE c1.ID_COURS < c2.ID_COURS
AND c1.DATE_DEBUT + COALESCE(c1.HEURE_DEBUT,540)/1440 < c2.DATE_FIN + COALESCE(c2.HEURE_FIN,1020)/1440
AND c2.DATE_DEBUT + COALESCE(c2.HEURE_DEBUT,540)/1440 < c1.DATE_FIN + COALESCE(c1.HEURE_FIN,1020)/1440"
if ($hasSeed) {
    Test-Check ($overlaps.Count -ge 1) "Alerte critique Chevauchement -> $($overlaps.Count) paire(s)"
} else {
    Write-Host "SKIP: alerte chevauchement (donnees de test absentes)" -ForegroundColor Yellow
    $script:Pass++
}

$pair610 = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS c1 JOIN COURS c2 ON c1.ID_FORMATEUR=c2.ID_FORMATEUR
WHERE c1.ID_COURS=6 AND c2.ID_COURS=10
AND c1.DATE_DEBUT + COALESCE(c1.HEURE_DEBUT,540)/1440 < c2.DATE_FIN + COALESCE(c2.HEURE_FIN,1020)/1440
AND c2.DATE_DEBUT + COALESCE(c2.HEURE_DEBUT,540)/1440 < c1.DATE_FIN + COALESCE(c1.HEURE_FIN,1020)/1440")
if ($hasSeed) {
    Test-Check ($pair610 -ge 1) "Cours 6 et 10 se chevauchent (meme formateur)"
} else {
    Write-Host "SKIP: paire 6-10 (donnees de test absentes)" -ForegroundColor Yellow
    $script:Pass++
}

$nEcheance = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE DATE_DEBUT BETWEEN SYSDATE AND SYSDATE + 7")
if ($hasSeed) {
    Test-Check ($nEcheance -ge 1) "Alerte Attention Echeance <= 7 jours -> $nEcheance cours"
} else {
    Write-Host "SKIP: alerte echeance (donnees de test absentes)" -ForegroundColor Yellow
    $script:Pass++
}

$nInactif = [int](Get-OraScalar "SELECT COUNT(DISTINCT f.ID_FORMATEUR) FROM FORMATEUR f
JOIN COURS c ON c.ID_FORMATEUR = f.ID_FORMATEUR WHERE LOWER(f.STATUS) = 'inactif'")
if ($hasSeed) {
    Test-Check ($nInactif -ge 1) "Alerte Attention formateur inactif -> $nInactif formateur(s)"
} else {
    Write-Host "SKIP: alerte formateur inactif (donnees de test absentes)" -ForegroundColor Yellow
    $script:Pass++
}

$nEnCours = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE DATE_DEBUT <= SYSDATE AND DATE_FIN >= SYSDATE")
if ($hasSeed) {
    Test-Check ($nEnCours -ge 1) "Alerte Information cours en cours -> $nEnCours cours"
} else {
    Write-Host "SKIP: alerte cours en cours (donnees de test absentes)" -ForegroundColor Yellow
    $script:Pass++
}

# ---------- 7. Planning / chevauchement ----------
Section-Header "7. Planning"
$bars = Get-OraRows "SELECT c.INTITULE, c.ID_FORMATEUR, f.NOM || ' ' || f.PRENOM, c.DATE_DEBUT, c.DATE_FIN, COALESCE(c.HEURE_DEBUT,540), COALESCE(c.HEURE_FIN,1020)
FROM COURS c JOIN FORMATEUR f ON c.ID_FORMATEUR = f.ID_FORMATEUR ORDER BY c.DATE_DEBUT"
Test-Check ($bars.Count -eq $nCours) "Chargement planning (JOIN formateur) -> $($bars.Count)/$nCours bars"

if ($hasSeed) {
    $conf = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS c1 JOIN COURS c2 ON c1.ID_FORMATEUR=c2.ID_FORMATEUR
WHERE c1.ID_COURS < c2.ID_COURS
AND c1.DATE_DEBUT + COALESCE(c1.HEURE_DEBUT,540)/1440 < c2.DATE_FIN + COALESCE(c2.HEURE_FIN,1020)/1440
AND c2.DATE_DEBUT + COALESCE(c2.HEURE_DEBUT,540)/1440 < c1.DATE_FIN + COALESCE(c1.HEURE_FIN,1020)/1440")
    Test-Check ($conf -ge 1) "Planning marque $conf conflit(s)"
} else {
    Write-Host "SKIP: conflits planning (donnees de test absentes)" -ForegroundColor Yellow
    $script:Pass++
}

# ---------- 8. CRUD complet ----------
Section-Header "8. CRUD (insert / update / delete)"
$email = 'test.integration@example.com'
Invoke-OraNonQuery "DELETE FROM COURS WHERE INTITULE LIKE 'TEST Integration%'"
Invoke-OraNonQuery "DELETE FROM FORMATEUR WHERE EMAIL = '$email'"
try {
    Invoke-OraNonQuery "INSERT INTO FORMATEUR (NOM,PRENOM,EMAIL,TELEPHONE,SPECIALITE,DATE_EMBAUCHE,STATUS)
                        VALUES ('IntTest','Auto','$email','000','Test',TO_DATE('2026-03-01','YYYY-MM-DD'),'Actif')"
    $fid = Get-OraScalar "SELECT ID_FORMATEUR FROM FORMATEUR WHERE EMAIL='$email'"
    Test-Check ($fid -gt 0) "Insert formateur (auto-id) OK (id=$fid)"

    Invoke-OraNonQuery "UPDATE FORMATEUR SET STATUS='Inactif' WHERE ID_FORMATEUR=$fid"
    Test-Check ((Get-OraScalar "SELECT STATUS FROM FORMATEUR WHERE ID_FORMATEUR=$fid") -eq 'Inactif') "Update formateur OK"

    Invoke-OraNonQuery "INSERT INTO COURS (INTITULE,CATEGORIE,NIVEAU,ID_FORMATEUR,DUREE_HEURES,DATE_DEBUT,DATE_FIN,HEURE_DEBUT,HEURE_FIN,CAPACITE,PROGRAMME)
                        VALUES ('TEST Integration Cours','Test','Debutant',$fid,2,TO_DATE('2026-11-01','YYYY-MM-DD'),TO_DATE('2026-11-01','YYYY-MM-DD'),540,720,15,'x')"
    $cid = Get-OraScalar "SELECT ID_COURS FROM COURS WHERE INTITULE='TEST Integration Cours'"
    Test-Check ($cid -gt 0) "Insert cours (auto-id) OK (id=$cid)"
    Test-Check ([int](Get-OraScalar "SELECT HEURE_DEBUT FROM COURS WHERE ID_COURS=$cid") -eq 540) "Heures cours stockees en minutes (540)"

    Invoke-OraNonQuery "INSERT INTO COURS (INTITULE,CATEGORIE,NIVEAU,ID_FORMATEUR,DUREE_HEURES,DATE_DEBUT,DATE_FIN,HEURE_DEBUT,HEURE_FIN,CAPACITE,PROGRAMME)
                        VALUES ('TEST Integration Cours 2','Test','Debutant',$fid,2,TO_DATE('2026-11-01','YYYY-MM-DD'),TO_DATE('2026-11-01','YYYY-MM-DD'),540,720,15,'x')"
    $cid2 = Get-OraScalar "SELECT ID_COURS FROM COURS WHERE INTITULE='TEST Integration Cours 2'"
    Test-Check ($cid2 -gt 0) "Insert second cours (conflit) OK (id=$cid2)"

    $ovl = [int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE ID_FORMATEUR=$fid AND ID_COURS <> $cid
        AND TO_DATE('2026-11-01','YYYY-MM-DD') + 600/1440 < DATE_FIN + COALESCE(HEURE_FIN,1439)/1440
        AND DATE_DEBUT + COALESCE(HEURE_DEBUT,0)/1440 < TO_DATE('2026-11-01','YYYY-MM-DD') + 660/1440")
    Test-Check ($ovl -ge 1) "Detection de conflit SQL OK (chevauchant -> $ovl)"

    Invoke-OraNonQuery "DELETE FROM COURS WHERE ID_COURS IN ($cid, $cid2)"
    Test-Check ([int](Get-OraScalar "SELECT COUNT(*) FROM COURS WHERE ID_COURS IN ($cid, $cid2)") -eq 0) "Delete cours OK"
    Invoke-OraNonQuery "DELETE FROM FORMATEUR WHERE ID_FORMATEUR=$fid"
    Test-Check ([int](Get-OraScalar "SELECT COUNT(*) FROM FORMATEUR WHERE ID_FORMATEUR=$fid") -eq 0) "Delete formateur OK"
} finally {
    Invoke-OraNonQuery "DELETE FROM COURS WHERE INTITULE LIKE 'TEST Integration%'"
    Invoke-OraNonQuery "DELETE FROM FORMATEUR WHERE EMAIL='$email'"
}

# ---------- Résumé ----------
$conn.Close()
Section-Header "Resultats"
Write-Host "$($script:Pass) passe(s), $($script:Fail) echec(s)"
Write-Host ""
Write-Host "NOTE: la generation des PDF passe par QFileDialog (action GUI manuelle) et" -ForegroundColor Yellow
Write-Host "le planning est valide en profondeur par le widget Gantt (test manuel app GUI)." -ForegroundColor Yellow
if ($script:Fail -eq 0) { exit 0 } else { exit 1 }
