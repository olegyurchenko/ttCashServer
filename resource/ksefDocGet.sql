--- Get Ksef documents list
select d.DAT_ID as id, DATETIME(d.TS) as 'time', c.T as type from TAG_C c
left join TAG_DAT d on d.DAT_ID = c.DAT_ID
{if paramCount}
where d.CR_ID in (
{loop param}
{param.comma}?
{end param}
)
{end paramCount}
order by c.C_ID desc limit 100
