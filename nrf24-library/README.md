### simple reliable and robust nrf24l01 library for stm32 built ardound stm32 hal framework

currently i dont see any perfect nrf24l01  library for stm32 boards. this library is very modular , robust , reliable and simple 
and it is built around stm32 HAL framework so you can use this with your project eaisly , currently it only support stm32cube ide but 
in future i am planning to do the same for arduino. you can find that this library have same implementation like HAL framwork (passing settings through pointer structure , every function return status like ok , busy , error, free) . because i am addicted to that these library are thread safe as it the function first check if the radio is free or busy in TX or RX .

all of the code is commented so you can easily identify the function property 

## SPI library 
a additional spi library so that you can port this library to other platform like esp32 , nrf52 framework.
you only need to change the spi library 

Feel free to do some changes and let me know 


<br  />
<br  />

### Connect with me:

[<img align="left" alt="codeSTACKr.com" width="22px" src="https://raw.githubusercontent.com/iconic/open-iconic/master/svg/globe.svg" />][website]
[<img align="left" alt="codeSTACKr | YouTube" width="22px" src="https://cdn.jsdelivr.net/npm/simple-icons@v3/icons/youtube.svg" />][youtube]
[<img align="left" alt="codeSTACKr | Twitter" width="22px" src="https://cdn.jsdelivr.net/npm/simple-icons@v3/icons/twitter.svg" />][twitter]
[<img align="left" alt="codeSTACKr | LinkedIn" width="22px" src="https://cdn.jsdelivr.net/npm/simple-icons@v3/icons/linkedin.svg" />][linkedin]
<!-- [<img align="left" alt="codeSTACKr | Instagram" width="22px" src="https://cdn.jsdelivr.net/npm/simple-icons@v3/icons/instagram.svg" />][instagram] -->

<br />



<!-- [<img align="left" alt="HTML5" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/html/html.png" />][webdevplaylist] -->
<!-- [<img align="left" alt="CSS3" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/css/css.png" />][cssplaylist]
[<img align="left" alt="Sass" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/sass/sass.png" />][cssplaylist]
[<img align="left" alt="JavaScript" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/javascript/javascript.png" />][jsplaylist]
[<img align="left" alt="React" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/react/react.png" />][reactplaylist]
[<img align="left" alt="Gatsby" width="26px" src="https://raw.githubusercontent.com/github/explore/e94815998e4e0713912fed477a1f346ec04c3da2/topics/gatsby/gatsby.png" />][webdevplaylist]
[<img align="left" alt="GraphQL" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/graphql/graphql.png" />][webdevplaylist]
[<img align="left" alt="Node.js" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/nodejs/nodejs.png" />][webdevplaylist]
[<img align="left" alt="Deno" width="26px" src="https://raw.githubusercontent.com/github/explore/361e2821e2dea67711cde99c9c40ed357061cf27/topics/deno/deno.png" />][webdevplaylist]
[<img align="left" alt="SQL" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/sql/sql.png" />][webdevplaylist]
[<img align="left" alt="MySQL" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/mysql/mysql.png" />][webdevplaylist]
[<img align="left" alt="MongoDB" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/mongodb/mongodb.png" />][webdevplaylist]
[<img align="left" alt="Git" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/git/git.png" />][webdevplaylist]
[<img align="left" alt="GitHub" width="26px" src="https://raw.githubusercontent.com/github/explore/78df643247d429f6cc873026c0622819ad797942/topics/github/github.png" />][webdevplaylist]
[<img align="left" alt="Terminal" width="26px" src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/terminal/terminal.png" />][webdevplaylist] -->

<br />
<br />


---
---

<!-- <det
</details> -->
<!-- 
<details>
  <summary>:zap: Github Stats</summary>

  <img align="left" alt="Himanshu's Github Stats" src="github-readme-stats.himanshu-4.vercel.app/api?username=Himanshu-4&show_icons=true&hide_border=true" />

</details> -->

[cpp]:https://code.visualstudio.com/docs/languages/cpp
[arduino]: https://www.arduino.cc/en/Main/software
[website]: https://jangragi123.wixsite.com/elector
[Twitter Follow]: https://twitter.com/Himansh09278011
[youtube]: https://www.youtube.com/channel/UCN3tGWHhQYkys3unVZjlOLA?disable_polymer=true
<!-- [instagram]: https://instagram.com/codeSTACKr -->
[linkedin]: https://www.linkedin.com/in/himanshu-jangra-b4865a180/
[Xinput]: https://www.partsnotincluded.com/how-to-emulate-an-xbox-controller-with-arduino-xinput/
[VScode]: https://code.visualstudio.com/
[blog]: https://jangragi123.wixsite.com/elector/post/how-to-make-an-xbox-360-controller
<!-- [webdevplaylist]: https://www.youtube.com/playlist?list=PLkwxH9e_vrAJ0WbEsFA9W3I1W-g_BTsbt
[jsplaylist]: https://www.youtube.com/playlist?list=PLkwxH9e_vrALRJKu7wfXby3MKeflhTu6B
[cssplaylist]: https://www.youtube.com/playlist?list=PLkwxH9e_vrALSdvZuEh6gqQdmDoDIoqz4
[reactplaylist]: https://www.youtube.com/playlist?list=PLkwxH9e_vrAK4TdffpxKY3QGyHCpxFcQ0 -->
