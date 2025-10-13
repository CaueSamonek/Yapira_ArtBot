# ArtBots da Equipe de Robótica Yapira da UFPR
## Flappy Sérgio 

# Organização Interna
## Fluxo de Desenvolvimento

1. **Selecione sua Issue**  
   Escolha a Issue em que vai trabalhar.  
   Para esse exemplo usaremos a Issue número **4**.  
   Acesse a Issue e atribua ela à você

2. **Garanta que o repositório local está atualizado**  
```bash
git checkout main
git pull
```

3. **Crie uma branch para sua Issue**  
   - O nome da branch deve seguir o formato `Issue#<número>`:  
   `git checkout -b Issue#4`

4. **Implemente suas modificações**  
   - Faça somente aquilo que a issue propõe para manter os commits/branches bem separados.

5. **Adicione e registre suas mudanças com mensagens claras e curtas**  
```bash
git checkout -b Issue#4
git add .  
git commit -m "Adicionado tutorial de desenvolvimento #4"
```

6. **Envie sua branch para o repositório remoto**  
   - Crie uma branch com o nome no formato `Issue#<número>`:  
```bash
git push origin Issue#4
```

7. **Abra um Pull Request**  
   - Vá até o repositório no GitHub.  
   - O GitHub sugerirá criar um Pull Request automaticamente.  
   - Verifique se o destino é a branch principal `main`.  

8. **Merge Na Main**
   - Faça o Merge se possível, caso dê conflito, corrija-os na sua branch e tente novamente.  
   - A Branch será automaticamente deletada  
   - A Issue deve ser fechada manualmente com uma descrição breve do que foi feito.  
