# 6.1: Envoyer une transaction avec une multisig

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La première façon de varier la façon dont vous envoyez une transaction de base est d'utiliser une multisig. Cela vous permet d'exiger que plusieurs personnes (ou au moins plusieurs clés privées) autorisent l'utilisation des fonds.

## Comprendre comment les multisigs fonctionnent

Dans une transaction P2PKH typique, les bitcoins sont envoyés à une adresse basée sur votre clé publique, ce qui signifie que la clé privée correspondante est nécessaire pour débloquer la transaction, résoudre le puzzle cryptographique et vous permettre de réutiliser les fonds. Mais que se passerait-il si vous pouviez, à la place, verrouiller une transaction avec _plusieurs_ clés privées ? Cela permettrait en effet d'envoyer des fonds à un groupe de personnes, qui devraient toutes accepter afin de pouvoir les réutiliser. 

_Qu'est-ce qu'une multi-signature ?_ Une multi-signature est une méthodologie qui permet à plusieurs personnes de créer conjointement une signature numérique. C'est une technique générale pour l'utilisation cryptographique des clés qui va bien au-delà de Bitcoin.

Techniquement, un puzzle cryptographique multi-signature est créé par Bitcoin à l'aide de la commande OP_CHECKMULTISIG, et est généralement encapsulé dans une adresse P2SH. [§8.4: Ecrire un script pour une multisig](08_4_Ecrire_un_script_pour_une_multisig.md) détaillera plus précisément le fonctionnement de cette commande. Pour l'instant, tout ce que vous devez savoir, c'est que vous pouvez utiliser la commande `bitcoin-cli` pour créer des adresses multi-signature ; les fonds peuvent être envoyés à ces adresses comme n'importe quelle adresse P2PKH normale, mais plusieurs clés privées seront nécessaires pour le rachat des fonds.

_Qu'est-ce qu'une transaction multi-signature ?_ Une transaction à signatures multiples est une transaction Bitcoin qui a été envoyée à une adresse à signatures multiples (multisig), nécessitant ainsi la signature de certaines personnes du groupe à signatures multiples afin de pouvoir réutiliser les fonds.

Les multi-signatures simples exigent que tous les membres du groupe signent les UTXO lorsqu'elles sont dépensées. Cependant, il est possible que la situation soit plus complexe. Les multi-signatures sont généralement décrites comme étant "m de n". Cela signifie que la transaction est verrouillée avec un groupe de "n" clés, mais que seuls "m" d'entre elles sont nécessaires pour déverrouiller la transaction. 

_Qu'est-ce qu'une multi-signature m-de-n ?_ Dans une multi-signature, "m" signatures d'un groupe de "n" sont nécessaires pour former la signature, où "m ≤ n".

## Créer une adresse multisig

Pour verrouiller un UTXO avec plusieurs clés privées, vous devez d'abord créer une adresse à plusieurs signatures. Les exemples utilisés ici montrent la création (et l'utilisation) d'une adresse multi-signature 2-de-2.

### Créer les adresses

Pour créer une adresse multisig, vous devez d'abord préparer les adresses P2PKH que le multisig va combiner. Les meilleures pratiques suggèrent de toujours créer de nouvelles adresses. Cela signifie que les participants exécuteront chacun la commande `getnewaddress` sur leur propre machine :

```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
Et:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Ensuite, l'un des destinataires (ou peut-être un tiers) devra recueillir les signatures.

#### Collecte des clés publiques

Cependant, vous n'avez pas besoin des adresses, qui sont les hachages des clés publiques, pour créer une multisig, mais plutôt des clés publiques elles-mêmes.

Ces informations sont facilement accessibles grâce à la commande `getaddressinfo`.

Sur la machine distante, que nous supposons ici être la `machine2`, vous pouvez obtenir les informations provenant de cette liste : 

```
machine2$ bitcoin-cli -named getaddressinfo address=$address2
{
  "address": "2N9Qnf7kGS5QX8mRDFQv7QWARFRqkKdp9pN",
  "scriptPubKey": "a914b15107009c65b631226d0626b22150098c91d35587",
  "ismine": true,
  "solvable": true,
  "desc": "sh(wpkh([801811ed/0'/0'/4']0373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f4))#rxfcwarv",
  "iswatchonly": false,
  "isscript": true,
  "iswitness": false,
  "script": "witness_v0_keyhash",
  "hex": "0014c06d895303bd7dff0320d7df9f33f99e8b9b0d93",
  "pubkey": "0373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f4",
  "embedded": {
    "isscript": false,
    "iswitness": true,
    "witness_version": 0,
    "witness_program": "c06d895303bd7dff0320d7df9f33f99e8b9b0d93",
    "pubkey": "0373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f4",
    "address": "tb1qcpkcj5crh47l7qeq6l0e7vlen69ekrvn509duc",
    "scriptPubKey": "0014c06d895303bd7dff0320d7df9f33f99e8b9b0d93"
  },
  "label": "",
  "ischange": false,
  "timestamp": 1579204237,
  "hdkeypath": "m/0'/0'/4'",
  "hdseedid": "67ffe46aa0cfd46eb342b78579f72fd1597833b4",
  "hdmasterfingerprint": "801811ed",
  "labels": [
    {
      "name": "",
      "purpose": "receive"
    }
  ]
}
```
L'adresse `pubkey` (`0373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f4`) est celle qui est requise. Copiez-la sur votre machine locale par le moyen qui vous semble le plus efficace et _le moins sujet aux erreurs_.

Ce processus doit être entrepris pour _chaque_ adresse provenant d'une machine autre que celle où le multisig est construit. Évidemment, si un tiers crée l'adresse, vous devrez le faire pour chaque adresse.

> :warning: **AVERTISSEMENT:** L'utilisation par Bitcoin de hachages de clés publiques comme adresses, au lieu de clés publiques, représente en fait une couche de sécurité supplémentaire. En effet, l'envoi d'une clé publique augmente légèrement la vulnérabilité de l'adresse associée, pour une possibilité lointaine de compromis avec la courbe elliptique. Vous ne devez pas vous inquiéter de devoir envoyer occasionnellement une clé publique pour une telle utilisation, mais vous devez savoir que les hachages de clés publiques représentent une sécurité, et que les clés publiques réelles ne doivent donc pas être envoyées n'importe comment.

Toutefois, si l'une des adresses a été créée sur votre machine locale, que nous supposons ici être la machine1, vous pouvez simplement transférer l'adresse `pubkey` dans une nouvelle variable.

```
machine1$ pubkey1=$(bitcoin-cli -named getaddressinfo address=$address1 | jq -r '.pubkey')
```

### Créer l'adresse

Une adresse multisig peut maintenant être créé avec la commande `createmultisig` :

```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","0373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f4"]'''
{
  "address": "2NDU6abQtzh4LcNs4Vd7WQJwZhkXSt1aZGM",
  "redeemScript": "522103f92e9f4c83f4438c86814952ab28836b6e3bfb38089a1f23ff8869eaf217982c210373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f452ae"
}
```
> :warning: **AVERTISSEMENT SUR LA VERSION:** Les anciennes versions de `createmultisig` vous permettaient de saisir une adresse au lieu d'une clé publique, si l'information complète sur l'adresse se trouvait dans votre portefeuille local. Ce n'est plus le cas pour la version moderne du noyau de Bitcoin, et cette technique ne doit donc pas être utilisée.

Lors de la création de l'adresse multisignature, vous indiquez le nombre de signatures requises avec l'argument `nrequired` (c'est-à-dire le "m" dans une multisignature "m-of-n"), puis vous indiquez l'ensemble des signatures possibles avec l'argument `keys` (c'est-à-dire le "n"). Notez que les entrées des clés proviennent probablement de différents endroits. Dans ce cas, nous avons inclus `$pubkey1` de la machine locale et `0373de7b25896556c33e7a6f5379151291d380c60b84c3ee9a8c933b08ce0da9f4` d'une machine distante. 

> :information_source: **NOTE — M-DE-N / N-DE-N:** Cet exemple montre la création d'une multisig simple 2-de-2. Si vous souhaitez plutôt créer une signature m-de-n où "m < n", vous ajustez le champ `nrequired` et/ou le nombre de signatures dans l'objet clé JSON. Pour une multisig 1-de-2, vous fixez `nrequired=1`, tandis que pour un multisigne 2-de-3, vous laissez `nrequired=2`, mais vous ajoutez une clé publique ou une adresse supplémentaire à la liste des clés.

Lorsqu'elle est utilisée correctement, `createmultisig` renvoie deux résultats, qui sont tous deux d'une importance capitale.

L'_adresse_ est celle que vous donnerez aux personnes qui veulent envoyer des fonds. Vous remarquerez qu'elle a un nouveau préfixe de 2, au lieu des préfixes que vous avez vus jusqu'à présent sur les adresses Bitcoin. C'est parce que `createmultisig` crée en fait un tout nouveau type d'adresse appelé adresse P2SH. Elle fonctionne exactement comme une adresse P2PKH standard pour l'envoi de fonds, mais vous devrez faire beaucoup plus de travail pour les dépenser.  

> :link: **TESTNET / MAINNET:** Sur testnet, le préfixe pour les adresses P2SH est de 2, tandis que sur mainnet, il est de 3.

Le _redeemScript_ est ce dont vous avez besoin pour racheter les fonds, ainsi que les clés privées pour le "m" des "n" adresses. Ce script est une autre particularité des adresses P2SH et sera expliqué en détail au [§8.1 : Comprendre les fondements du P2SH](08_1_Comprendre_les_fondements_du_P2SH.md). Pour l'instant, sachez simplement qu'il s'agit d'un peu de données nécessaires pour obtenir votre argent.

_Qu'est-ce qu'une adresse P2SH ?_ P2SH signifie Pay-to-script-hash. C'est un type de destinataire différent d'une adresse P2PKH standard, utilisée pour les fonds dont le rachat est basé sur des scripts Bitcoin plus complexes. `bitcoin-cli` utilise l'encapsulation P2SH pour aider à standardiser et simplifier ses multisigs comme "P2SH multisigs".

> :warning: **AVERTISSEMENT:** Les adresses P2SH multisig, comme celles créées par `bitcoin-cli`, ont une limite pour "m" et "n" en multisigs basée sur la taille maximale du redeemScript, qui est actuellement de 520 octets. En pratique, vous n'atteindrez pas cette limite, à moins que vous ne fassiez quelque chose d'excessif.

### Sauvegarder votre travail

Mise en garde importante : rien de ce qui concerne votre multisig n'est enregistré dans votre portefeuille en utilisant ces techniques de base. Afin de pouvoir rembourser plus tard l'argent envoyé à cette adresse multisig, vous devrez conserver deux informations cruciales :

   * Une liste des adresses Bitcoin utilisées dans le multisig.
   * La sortie  `redeemScript` créée par `createmultsig`.

Techniquement, le `redeemScript` peut être recréé en relançant `createmultisig` avec la liste complète des adresses et/ou des clés publiques _dans le même ordre_ et avec le bon compte m-of-n. Mais il vaut mieux le conserver et s'épargner ainsi du stress et du chagrin.

## Envoyer à une adresse Multisig

Si vous disposez d'une adresse multisig dans un format P2SH pratique, comme celui généré par `bitcoin-cli`, elle peut être envoyée exactement comme une adresse normale.

```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.2995}''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "ad16ea68a62af2d3930a48c5ca811bf66935f768bb369a85298ee6697167c667",
  "hash": "ad16ea68a62af2d3930a48c5ca811bf66935f768bb369a85298ee6697167c667",
  "size": 83,
  "vsize": 83,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "ad16098f5c8904a4de7c152efc56359c22be37d447cd78019c398791a7bdd928",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 1.29950000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL",
        "hex": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"
        ]
      }
    }
  ]
}
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328
```
Comme vous pouvez le constater, la création de la transaction n'avait rien d'inhabituel et elle semblait tout à fait normale, même si l'adresse avait un préfixe différent de la normale (`2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz`).

## Résumé : Envoyer une transaction avec un multisig

Les adresses multisig lient les fonds à plusieurs clés privées - nécessitant éventuellement toutes ces clés privées pour le rachat, et éventuellement n'en exigeant qu'une partie de l'ensemble. Elles sont assez faciles à créer avec `bitcoin-cli` et il est tout à fait normal de les envoyer. Cette facilité est due en grande partie à l'utilisation invisible des adresses P2SH (pay-to-script-hash), un vaste sujet qui sera davantage couvert à l'avenir.

_Quel est le pouvoir des multi-signatures ?_ Les multi-signatures permettent de modéliser une variété d'arrangements financiers tels que les sociétés, les partenariats, les comités et autres groupes. Une multisig 1 de 2 peut être le compte bancaire commun d'un couple marié, tandis qu'un multisig 2 de 2 peut être utilisé pour les grosses dépenses d'une société à responsabilité limitée. Les multisig constituent également l'une des bases des contrats intelligents. Par exemple, une transaction immobilière peut être conclue avec une multisig 2 de 3, où les signatures sont soumises par l'acheteur, le vendeur et un agent fiduciaire. Une fois que l'agent fiduciaire accepte que toutes les conditions sont remplies, il libère les fonds pour le vendeur ; ou bien, l'acheteur et le vendeur peuvent libérer conjointement les fonds.

## Quelle est la prochaine étape ?

Continuez "Expansion des transactions Bitcoin" avec le [§6.2 : Dépenser une transaction avec une multisig](06_2_Depenser_une_transaction_avec_une_multisig.md).