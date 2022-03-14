-- uart.vhd: UART controller - receiving part
--
-- Project: INC - UART Receiver
-- Author:	Evgeny Torbin
-- Faculty:	Information Technology, Brno University of Technology
-- Date:	06.05.2020
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all; -- pro prevod std_logic_vector do integeru

-------------------------------------------------
entity UART_RX is
port(

	-- Vstupni signaly --
    CLK			: in std_logic;
    RST			: in std_logic;
    DIN			: in std_logic;
    
    -- Vystupni signaly a definice --
    DOUT		: out std_logic_vector(7 downto 0) 	:= "00000000";
    DOUT_VLD	: out std_logic 					:= '0'
    
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is

-- Definice signalu --
signal clk_cnt    	: std_logic_vector(4 downto 0); -- pocit
signal bit_cnt   	: std_logic_vector(3 downto 0);

-- Pokud se rovna 1:
signal rx_enabled  	: std_logic; -- nacitame data
signal cnt_enabled 	: std_logic; -- pocitame takty hodinoveho signalu
signal data_valid	: std_logic; -- nastavime DOUT_VLD
    
begin

	-- Propojeni signalu entit UART_FSM a UART_RX --
    FSM: entity work.UART_FSM(behavioral)
    port map (
        i_CLK			=> CLK,
        i_RST			=> RST,
        i_DIN			=> DIN,
        i_CLK_CNT		=> clk_cnt,
       	i_BIT_CNT		=> bit_cnt,
        o_RX_ENABLED	=> rx_enabled,
        o_CNT_ENABLED	=> cnt_enabled,
        o_DATA_VALID	=> data_valid
    );
    
    -- Hlavni proces --
    main: process (CLK) begin
    	-- Pri kazdem taktu hodinoveho signalu --
        if rising_edge(CLK) then
        	
        	-- Spravnost nactene hodnoty --
        	-- Nastavime DOUT_VLD vystup na hodnotu data_valid
        	DOUT_VLD <= data_valid;
        	
        	-- Pocitani taktu --
        	-- Pokud mame pocitat takty hodinovych signalu,
        	-- tak pricteme zvetsime clk_cnt o jednicku, jinak vynulujeme
            if cnt_enabled = '1' then
                clk_cnt <= clk_cnt + 1;
            else
                clk_cnt <= "00000";
            end if;
            
            -- Nacitani dat --
            -- Kdyz mame nacitat data, tak ...
            if rx_enabled = '1' then
            
            	-- Pokud 5 bajt clk_cnt se rovna 1 =>
            	-- => cislo v clk_cnt >= 16, tedy
            	-- vypiseme bit na pozici bit_cnt, vynulujeme clk_cnt
            	-- a zvetsime bit_cnt o jednicku
                if clk_cnt(4) = '1' then
                
                    DOUT(to_integer(unsigned(bit_cnt))) <= DIN; -- pouzivame funkce to_integer() z knihovny numeric_std
                    											-- pro prevod std_logic_vector do integeru
                    											
                    clk_cnt <= "00000";
                    bit_cnt <= bit_cnt + 1;
                    
                end if;
                
            -- Jinak vynulujeme bit_cnt
            else bit_cnt <= "0000"; end if;
            
        end if;
    end process;
    
end behavioral;
