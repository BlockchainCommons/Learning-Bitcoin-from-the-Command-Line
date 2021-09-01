# 20.4: Expandindo a Lightning Network

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esses dois capítulos cobriram apenas algumas das atividades mais importantes da Lightning. Há muito mais que pode ser feito e muitas variedades possíveis. A seguir, daremos algumas dicas importantes.

## Usando Plugins c-lightning

O c-lightning é uma implementação leve, altamente personalizável e compatível com o padrão do protocolo Lightning Network. Ele estende a funcionalidade usando plugins. Principalmente, esses são subprocessos que são iniciados pelo daemon `lightningd` e podem interagir com o `lightningd` de várias maneiras:

* As opções de linha de comando permitem que os plugins registrem os próprios argumentos usando a linha de comando, que são então expostos por meio do `lightningd`;
* A passagem de comando JSON-RPC permite que os plugins adicionem os próprios comandos à interface JSON-RPC;
* As assinaturas de fluxo de eventos fornecem plug-ins com um mecanismo de notificação baseados em push para o `lightnind`;
* Hooks são uma opção primitiva que permite que os plugins sejam notificados sobre eventos no daemon `lightningd` e modifiquem o comportamento ou transmitam comportamentos personalizados.

Um plugin pode ser escrito em qualquer linguagem e pode se comunicar com o `lightningd` através do stdin e stdout do plugin. O JSON-RPCv2 é usado como protocolo no topo dos dois fluxos, com o plugin atuando como servidor e `lightningd` atuando como cliente.

O repositório `lightningd` GitHub mantém uma lista atualizada de [plugins](https://github.com/lightningd/plugins) disponíveis.

## Usando Carteiras Mobile

Atualmente, sabemos de duas carteiras de dispositivos móveis da Lightning que suportam a implementação do c-lightning.

Para dispositivos iOS, o FullyNoded é uma carteira de Bitcoin iOS open source que se conecta através do serviço autenticado Tor V3 ao nosso próprio full node. A funcionalidade FullyNoded está atualmente em desenvolvimento ativo e na fase beta inicial de testes.

* [FullyNoded](https://github.com/Fonta1n3/FullyNoded/blob/master/Docs/Lightning.md)

O SparkWallet é uma carteira GUI minimalista para a c-lightning, acessível pela web ou por meio de aplicativos móveis e de desktop para Android.

* [SparkWallet](https://github.com/shesek/spark-wallet)

## Usando Diferentes Implementações da Lightning

O c-lightning não é a nossa única opção. Hoje, existem três implementações amplamente utilizadas para a Lightning Network. Todos seguem as [Documentações Base para a Tecnologia Lightning (BOLT)](https://github.com/lightningnetwork/lightning-rfc), que descrevem um protocolo de segunda camada para transferências de bitcoins off-chain. As especificações são atualmente um trabalho em andamento que ainda está sendo elaborado.

| Nome  | Descrição | BitcoinStandup | Linguagem | Repositório |
| ------------- | ------------- | :---: | ------------- | ------------- |
| C-lighting  | Blockstream  | X | C | [Download](https://github.com/ElementsProject/lightning) |
| LND  | Lightning Labs  | X | Go | [Download](https://github.com/lightningnetwork/lnd) |
| Eclair  | ACINQ  | - | Scala | [Download](https://github.com/ACINQ/eclair) |

## Fazendo Backups

Nosso node Lightning precisa estar online o tempo todo, caso contrário, nossa contraparte pode enviar um status de canal anterior e roubar nossos fundos. No entanto, há outro cenário em que os fundos podem ser perdidos, que é quando ocorre uma falha no hardware que impede o node de estabelecer um fechamento cooperativo com a contraparte. Isso provavelmente significará que, se não tivermos uma cópia exata do estado do canal antes da falha, teremos um estado inválido que pode fazer com que o outro node o considere como uma tentativa de fraude e use a transação penalizada. Nesse caso, todos os fundos serão perdidos. Para evitar esta situação indesejável, uma solução baseada na alta disponibilidade do banco de dados postgreSQL [existe](https://github.com/gabridome/docs/blob/master/c-lightning_with_postgresql_reliability.md).

PS: Não testamos esta solução.

## Resumo: Expandindo a Lightning Network

Podemos usar diferentes implementações, plugins, carteiras para celular e backups para expandir nossa experiência com a Lightning.

## O Que Vem Depois?

Concluímos todo o livro Aprendendo sobre o Bitcoin usando a Linha de Comando. Embora não precisemos visitar os [Apêndices](A0_Appendices.md), que possuem configurações alternativas, podemos fazer isso agora.

Caso contrário, nós o encorajamos a ingressar nas comunidades de desenvolvedores e programadores e também, colocar seu novo conhecimento para funcionar.

Você também pode nos ajudar aqui na Blockchain Commons com issues ou PRs para Aprendendo Bitcoin ou para qualquer um de nossos outros repositórios, ou pode até mesmo se tornar um [patrocinador](https://github.com/sponsors/BlockchainCommons). Também pode ajudar divulgando o trabalho, contando às pessoas nas redes sociais sobre o curso e o que aprendeu com ele!

Agora vá lá e faça da comunidade do Bitcoin um lugar melhor!